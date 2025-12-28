import torch
import torch.nn as nn
import snntorch as snn
from snntorch import surrogate
import torch.nn.functional as F
from torchvision import transforms
from torch.utils.data import DataLoader
import cv2
import numpy as np
import os
import shutil

# --- CONFIGURATION ---
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
TIME_STEPS = 8
RESIZE_DIM = (128, 128)
N_EMBD = 128
VOCAB = {"Calme": 0, "Mouvement_Rapide": 1, "Obstacle_Proche": 2}
ITOS = {v: k for k, v in VOCAB.items()}

# --- 1. CHARGEMENT ET MAPPING DU DATASET ---
print("Chargement du dataset depuis Hugging Face...")
print("Loading dataset with manual video decoding...")

from huggingface_hub import hf_hub_download
from datasets import Dataset
import json

print("Downloading dataset metadata and files...")

def create_manual_dataset():
    """Create dataset by directly downloading and processing video files"""
    from huggingface_hub import list_repo_files, hf_hub_url
    import requests
    
    repo_id = "sayakpaul/ucf101-subset"
    
    # Get list of files in the repository
    try:
        files = list_repo_files(repo_id)
        video_files = [f for f in files if f.endswith('.avi')]
        
        print(f"Found {len(video_files)} video files")
        
        # Limit to first 20% for training
        num_videos = max(10, len(video_files) // 5)
        video_files = video_files[:num_videos]
        
        # Download videos to a local cache
        cache_dir = os.path.expanduser("~/.cache/ucf101_manual")
        os.makedirs(cache_dir, exist_ok=True)
        
        dataset_items = []
        for video_file in video_files:
            try:
                # Download video file
                local_path = hf_hub_download(
                    repo_id=repo_id,
                    filename=video_file,
                    repo_type="dataset",
                    cache_dir=cache_dir
                )
                
                # Format: v_ActionName_gXX_cXX.avi
                action_name = video_file.split('_')[1] if '_' in video_file else "Unknown"
                
                label = 0  # Default "Calme"
                if any(fast_word in action_name.lower() for fast_word in ['dunk', 'jump', 'run', 'kick']):
                    label = 1  # "Mouvement_Rapide"
                
                dataset_items.append({
                    'video_path': local_path,
                    'label': label,
                    'action': action_name
                })
                
                print(f"Downloaded: {video_file} -> {action_name} (label={label})")
                
            except Exception as e:
                print(f"Error downloading {video_file}: {e}")
                continue
        
        return dataset_items
        
    except Exception as e:
        print(f"Error accessing repository: {e}")
        return []

try:
    dataset_items = create_manual_dataset()
    if len(dataset_items) == 0:
        raise RuntimeError("No videos could be downloaded")
    print(f"Successfully prepared {len(dataset_items)} videos for training")
    
except Exception as e:
    print(f"Error creating manual dataset: {e}")
    raise RuntimeError("Could not load dataset. Please check your internet connection and try again.")

def get_label_mapping(label_value):
    return label_value

# --- 2. PRE-PROCESSING : TEMPORAL DIFF + SNN ---
class VideoDataset(torch.utils.data.Dataset):
    def __init__(self, dataset_items):
        self.items = dataset_items
        self.transform = transforms.Compose([
            transforms.ToPILImage(),
            transforms.Resize(RESIZE_DIM),
            transforms.Grayscale(),
            transforms.ToTensor(),
        ])

    def __len__(self):
        return len(self.items)
    
    def _load_video_with_cv2(self, video_path):
        """Load video using cv2"""
        cap = cv2.VideoCapture(video_path)
        frames = []
        
        while True:
            ret, frame = cap.read()
            if not ret:
                break
            # BGR to RGB
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            frames.append(frame)
        
        cap.release()
        return frames

    def __getitem__(self, idx):
        try:
            item = self.items[idx]
            video_path = item['video_path']
            label = item['label']
            
            video_frames = self._load_video_with_cv2(video_path)
            
            if len(video_frames) < TIME_STEPS + 1:
                print(f"Warning: Video {idx} has only {len(video_frames)} frames, need {TIME_STEPS + 1}")
                if len(video_frames) > 0:
                    frame = self.transform(video_frames[0])
                else:
                    frame = torch.zeros(1, RESIZE_DIM[0], RESIZE_DIM[1])
                processed_frames = [frame * 0] * TIME_STEPS  # Zero deltas
                return torch.stack(processed_frames), torch.tensor(label)
            
            # Extraction de TIME_STEPS frames + Calcul de la différence
            processed_frames = []
            frame_indices = np.linspace(0, len(video_frames) - 1, TIME_STEPS + 1, dtype=int)
            
            for i in range(1, len(frame_indices)):
                f_current = self.transform(video_frames[frame_indices[i]])
                f_prev = self.transform(video_frames[frame_indices[i-1]])
                # Delta Frame : On ne garde que ce qui a changé
                delta = torch.abs(f_current - f_prev)
                processed_frames.append(delta)
                
            return torch.stack(processed_frames), torch.tensor(label)
            
        except Exception as e:
            print(f"Error loading video {idx}: {e}")
            # Return dummy data
            dummy_frame = torch.zeros(1, RESIZE_DIM[0], RESIZE_DIM[1])
            processed_frames = [dummy_frame] * TIME_STEPS
            return torch.stack(processed_frames), torch.tensor(0)

# --- 3. LE MODÈLE SpikingVLM (Optimisé pour le Delta) ---
class SpikingVLM(nn.Module):
    def __init__(self):
        super().__init__()
        # Encodeur SNN : Réagit uniquement aux changements (Delta)
        self.conv = nn.Conv2d(1, 32, kernel_size=3, stride=2)
        self.lif = snn.Leaky(beta=0.6, threshold=1.0, spike_grad=surrogate.fast_sigmoid())
        
        # Bottleneck
        self.flatten = nn.Flatten()
        # 128x128 -> Conv 3x3 s2 -> 63x63
        self.proj = nn.Linear(32 * 63 * 63, N_EMBD)
        
        # Décodeur Transformer
        self.decoder_layer = nn.TransformerDecoderLayer(d_model=N_EMBD, nhead=8, batch_first=True)
        self.transformer = nn.TransformerDecoder(self.decoder_layer, num_layers=3)
        self.head = nn.Linear(N_EMBD, len(VOCAB))

    def forward(self, x):
        # x: [Batch, Steps, 1, H, W]
        b, s, c, h, w = x.shape
        mem = self.lif.init_leaky()
        spk_rec = []

        for step in range(s):
            cur = self.conv(x[:, step])
            spk, mem = self.lif(cur, mem)
            spk_rec.append(spk)
        
        # On aggrège l'activité neuronale
        combined_spikes = torch.stack(spk_rec).mean(dim=0)
        z = self.flatten(combined_spikes)
        vis_emb = F.relu(self.proj(z)).unsqueeze(1)
        
        # Le transformer "lit" le résumé des spikes
        tgt = torch.zeros_like(vis_emb)
        out = self.transformer(tgt, vis_emb)
        return self.head(out.squeeze(1))

# --- 4. BOUCLE D'ENTRAÎNEMENT ---
def run_training():
    train_ds = VideoDataset(dataset_items)
    loader = DataLoader(train_ds, batch_size=4, shuffle=True, num_workers=0)  # num_workers=0 for debugging
    
    model = SpikingVLM().to(DEVICE)
    optimizer = torch.optim.AdamW(model.parameters(), lr=1e-4)
    criterion = nn.CrossEntropyLoss()

    print("Démarrage de l'entraînement sur données réelles...")
    model.train()
    for epoch in range(5):
        total_loss = 0.0
        batch_count = 0
        for i, (vids, labels) in enumerate(loader):
            vids, labels = vids.to(DEVICE), labels.to(DEVICE)
            
            logits = model(vids)
            loss = criterion(logits, labels)
            
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            
            total_loss += loss.item()
            batch_count += 1
            
            if i % 10 == 0:
                print(f"Epoch {epoch} | Batch {i} | Loss: {loss.item():.4f}")
        
        avg_loss = total_loss / batch_count if batch_count > 0 else 0
        print(f"Epoch {epoch} completed | Avg Loss: {avg_loss:.4f}")

    torch.save(model.state_dict(), "vlm_motion_expert.pth")
    print("Modèle expert sauvegardé !")

if __name__ == "__main__":
    run_training()