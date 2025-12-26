import torch
import torch.nn as nn
import snntorch as snn
from snntorch import surrogate
import torch.nn.functional as F
import cv2
import numpy as np
import time

DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
BATCH_SIZE = 1
TIME_STEPS = 5
VOCAB = {"<PAD>": 0, "Calme": 1, "Mouvement_Rapide": 2, "Obstacle_Proche": 3}
ITOS = {v: k for k, v in VOCAB.items()}
N_EMBD = 64

class SpikingVLM(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv = nn.Conv2d(1, 16, kernel_size=5, stride=2)
        self.bn = nn.BatchNorm2d(16)
        self.lif = snn.Leaky(beta=0.9, threshold=1.0, spike_grad=surrogate.fast_sigmoid())
        
        self.flatten = nn.Flatten()
        self.proj = nn.Linear(16 * 62 * 62, N_EMBD)
        self.dropout = nn.Dropout(0.2)
        
        self.decoder_layer = nn.TransformerDecoderLayer(d_model=N_EMBD, nhead=4, batch_first=True, dropout=0.1)
        self.decoder = nn.TransformerDecoder(self.decoder_layer, num_layers=2)
        self.output_head = nn.Linear(N_EMBD, len(VOCAB))
        
        self._init_weights()
    
    def _init_weights(self):
        nn.init.xavier_uniform_(self.conv.weight)
        nn.init.xavier_uniform_(self.proj.weight)
        nn.init.xavier_uniform_(self.output_head.weight)
        nn.init.zeros_(self.output_head.bias)

    def forward(self, video_seq):
        mem = self.lif.init_leaky()
        spk_sum = 0
        
        for step in range(video_seq.size(0)):
            cur = self.bn(self.conv(video_seq[step]))
            spk, mem = self.lif(cur, mem)
            spk_sum += spk
            
        x = self.flatten(spk_sum)
        vis_emb = self.dropout(torch.relu(self.proj(x))).unsqueeze(1)
        
        tgt = torch.zeros((video_seq.size(1), 1, N_EMBD)).to(DEVICE)
        out = self.decoder(tgt, vis_emb)
        logits = self.output_head(out)
        return logits

def get_dummy_data():
    dummy_video = torch.randn(TIME_STEPS, 1, 1, 128, 128).to(DEVICE)
    label = torch.randint(1, len(VOCAB), (1,)).to(DEVICE)
    return dummy_video, label

def capture_training_data(label_name, num_samples=5):
    cap = cv2.VideoCapture(0)
    samples = []
    print(f"Préparez la scène pour : {label_name}. Appuyez sur 'c' pour capturer.")
    
    while len(samples) < num_samples:
        ret, frame = cap.read()
        if not ret:
            print("Erreur de lecture de la caméra")
            break
        cv2.imshow('Capture', frame)
        if cv2.waitKey(1) & 0xFF == ord('c'):
            seq = []
            for _ in range(TIME_STEPS):
                ret, f = cap.read()
                if not ret:
                    break
                gray = cv2.cvtColor(f, cv2.COLOR_BGR2GRAY)
                res = cv2.resize(gray, (128, 128))
                seq.append(torch.FloatTensor(res).unsqueeze(0) / 255.0)
            if len(seq) == TIME_STEPS:
                samples.append(torch.stack(seq))
                print(f"Echantillon {len(samples)} capturé !")
            
    cap.release()
    cv2.destroyAllWindows()
    return torch.stack(samples) if samples else None

def train_model(model, epochs=20):
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-4, weight_decay=1e-5)
    scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(optimizer, patience=5, factor=0.5)
    model.train()
    
    for e in range(epochs):
        total_loss = 0
        correct = 0
        total = 0
        
        for _ in range(10):
            video, label = get_dummy_data()
            logits = model(video).squeeze(1)
            loss = F.cross_entropy(logits, label)
            
            optimizer.zero_grad()
            loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
            optimizer.step()
            
            total_loss += loss.item()
            pred = torch.argmax(logits, dim=-1)
            correct += (pred == label).sum().item()
            total += label.size(0)
            
        avg_loss = total_loss / 10
        acc = 100 * correct / total
        scheduler.step(avg_loss)
        
        if e % 5 == 0:
            print(f"Epoch {e} - Loss: {avg_loss:.4f} - Acc: {acc:.2f}%")

    torch.save(model.state_dict(), "fast_vlm.pth")
    print("Modèle sauvegardé dans fast_vlm.pth")

def train_balanced(model, data_dict, epochs=50):
    optimizer = torch.optim.Adam(model.parameters(), lr=1e-4, weight_decay=1e-5)
    scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(optimizer, T_max=epochs)
    model.train()
    
    labels = list(data_dict.keys())
    
    for e in range(epochs):
        total_loss = 0
        correct = 0
        total = 0
        
        np.random.shuffle(labels)
        
        for label_idx in labels:
            sequences = data_dict[label_idx]
            idx = np.random.randint(0, len(sequences))
            video = sequences[idx].unsqueeze(1).to(DEVICE)
            target = torch.tensor([label_idx]).to(DEVICE)
            
            logits = model(video).squeeze(1)
            loss = F.cross_entropy(logits, target)
            
            optimizer.zero_grad()
            loss.backward()
            torch.nn.utils.clip_grad_norm_(model.parameters(), 1.0)
            optimizer.step()
            
            total_loss += loss.item()
            pred = torch.argmax(logits, dim=-1)
            correct += (pred == target).sum().item()
            total += 1
            
        scheduler.step()
        avg_loss = total_loss / len(data_dict)
        acc = 100 * correct / total
        
        if e % 10 == 0:
            print(f"Epoch {e} - Loss: {avg_loss:.4f} - Acc: {acc:.2f}%")

    torch.save(model.state_dict(), "fast_vlm_balanced.pth")
    print("Modèle équilibré sauvegardé dans fast_vlm_balanced.pth")

def start_inference(model_path="fast_vlm.pth"):
    model = SpikingVLM().to(DEVICE)
    try:
        model.load_state_dict(torch.load(model_path, map_location=DEVICE))
        print(f"Modèle chargé depuis {model_path}")
    except FileNotFoundError:
        print(f"Modèle {model_path} non trouvé. Veuillez d'abord entraîner le modèle.")
        return
    model.eval()
    
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Erreur : impossible d'ouvrir la caméra")
        return
        
    print("Caméra activée. Analyse en cours... (Appuyez sur 'q' pour quitter)")
    
    frame_buffer = []
    
    while True:
        ret, frame = cap.read()
        if not ret: 
            break
        
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        resized = cv2.resize(gray, (128, 128))
        tensor_frame = torch.FloatTensor(resized).unsqueeze(0).unsqueeze(0).to(DEVICE) / 255.0
        
        frame_buffer.append(tensor_frame)
        
        if len(frame_buffer) == TIME_STEPS:
            input_seq = torch.stack(frame_buffer)
            
            with torch.no_grad():
                logits = model(input_seq)
                probs = F.softmax(logits, dim=-1).squeeze()
                pred_idx = torch.argmax(probs).item()
                confidence = probs[pred_idx].item() * 100
                analyse = ITOS[pred_idx]
            
            cv2.putText(frame, f"Analyse: {analyse} ({confidence:.1f}%)", (20, 50), 
                        cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            frame_buffer.pop(0)
            
        cv2.imshow('Robot Eyes - SNN+VLM', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'): 
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    import sys
    
    print("=== SNN+VLM Robot Vision ===")
    print("1. Entraînement simple (données factices)")
    print("2. Entraînement équilibré (capture réelle)")
    print("3. Inférence temps réel")
    choice = input("Choisissez une option (1/2/3): ")
    
    if choice == "1":
        net = SpikingVLM().to(DEVICE)
        train_model(net, epochs=50)
        print("Entraînement terminé. Vous pouvez maintenant lancer l'inférence (option 3)")
        
    elif choice == "2":
        print("\n=== Capture de données d'entraînement ===")
        data_calme = capture_training_data("CALME", num_samples=10)
        data_mouvement = capture_training_data("MOUVEMENT", num_samples=10)
        
        if data_calme is not None and data_mouvement is not None:
            data_dict = {
                VOCAB["Calme"]: data_calme,
                VOCAB["Mouvement_Rapide"]: data_mouvement
            }
            net = SpikingVLM().to(DEVICE)
            train_balanced(net, data_dict, epochs=200)
            print("Entraînement équilibré terminé. Vous pouvez maintenant lancer l'inférence (option 3)")
        else:
            print("Erreur lors de la capture des données")
            
    elif choice == "3":
        model_path = input("Chemin du modèle (défaut: fast_vlm.pth): ").strip()
        if not model_path:
            model_path = "fast_vlm.pth"
        start_inference(model_path)
        
    else:
        print("Option invalide")