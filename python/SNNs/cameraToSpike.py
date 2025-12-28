import cv2
import torch
import torch.nn as nn
import snntorch as snn
from snntorch import surrogate
import numpy as np

# --- CONFIGURATION ---
# Utilisez 'cuda' si vous avez un GPU Nvidia, sinon 'cpu' (suffisant pour ce petit exemple)
DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
RESIZE_DIM = (160, 160) # Taille optimale pour équilibre détail/performance
THRESHOLD = 1.0         # Sensibilité du neurone optimale (plus de spikes visibles)
BETA = 0.5              # Décroissance rapide pour meilleure réactivité temporelle
MIN_SPIKE_THRESHOLD = 0.3  # Seuil optimal pour capturer l'activité neuronale

print(f"Utilisation du périphérique : {DEVICE}")

# --- 1. DÉFINITION DE L'ENCODEUR SNN ---
class RealTimeSpikeEncoder(nn.Module):
    def __init__(self):
        super().__init__()
        
        # Étape A : Une petite convolution pour extraire des traits
        # Entrée: 1 canal (gris), Sortie: 4 canaux de caractéristiques
        # Kernel 3x3 pour détecter des petits détails
        self.conv = nn.Conv2d(1, 4, kernel_size=3, padding=1, bias=False)
        
        # Étape B : Le neurone SNN (LIF)
        # spike_grad est nécessaire pour l'entraînement, moins pour l'inférence
        # mais on le laisse pour la bonne forme.
        self.lif = snn.Leaky(beta=BETA, threshold=THRESHOLD, spike_grad=surrogate.fast_sigmoid())

    def forward(self, x, mem):
        # 1. L'image devient du courant électrique via la convolution
        current = self.conv(x) 
        
        # 2. Le neurone intègre le courant et son état passé (mem)
        # Il génère un spike (spk) si le seuil est dépassé, et met à jour sa mémoire.
        spk, mem = self.lif(current, mem)
        
        return spk, mem

# --- 2. PRÉPARATION ---
# Charger le modèle et le mettre sur le périphérique (CPU/GPU)
spike_encoder = RealTimeSpikeEncoder().to(DEVICE)
# Mettre en mode évaluation (important même si on n'entraîne pas ici)
spike_encoder.eval() 

# Initialiser l'état de mémoire des neurones (au début, le potentiel est à 0)
# Le '1' est pour la dimension du batch
mem_state = spike_encoder.lif.init_leaky().to(DEVICE)

# Démarrer la caméra
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Erreur: Impossible d'ouvrir la caméra.")
    exit()

print("Appuyez sur 'q' pour quitter.")

# --- 3. BOUCLE TEMPS RÉEL ---
with torch.no_grad(): # Pas besoin de calculer les gradients pour l'inférence
    while True:
        ret, frame = cap.read()
        if not ret: break

        # --- A. Pré-traitement de l'image ---
        # Convertir en niveaux de gris
        gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        # Appliquer un filtre gaussien pour réduire le bruit - NOUVEAU
        denoised_frame = cv2.GaussianBlur(gray_frame, (5, 5), 1.5)
        
        # Redimensionner pour le SNN (128x128 est rapide)
        small_frame = cv2.resize(denoised_frame, RESIZE_DIM)
        
        # Convertir en tenseur PyTorch : [Batch, Channel, Height, Width]
        # Normaliser les pixels entre 0.0 et 1.0
        input_tensor = torch.from_numpy(small_frame).float().unsqueeze(0).unsqueeze(0).to(DEVICE) / 255.0

        # --- B. Passage dans le SNN ---
        # Important : .detach() sur la mémoire pour éviter que PyTorch 
        # n'essaie de stocker l'historique infini en temps réel.
        mem_state = mem_state.detach()
        spikes_out, mem_state = spike_encoder(input_tensor, mem_state)

        # --- C. Visualisation des Spikes ---
        # spikes_out a la forme [1, 4, 128, 128] (Batch, 4 canaux, H, W)
        # On veut une seule image 2D. On fait la somme des activités des 4 canaux.
        spike_activity = spikes_out.sum(dim=1).squeeze().cpu().numpy()
        
        # Création de l'image visuelle avec filtrage du bruit - MODIFIÉ
        # Seuil plus élevé pour éliminer les spikes faibles
        spike_vis_img = (spike_activity > MIN_SPIKE_THRESHOLD).astype(np.uint8) * 255
        
        # On agrandit l'image des spikes à la taille de la vidéo originale
        # INTER_NEAREST garde l'aspect "pixelisé/numérique" des spikes
        spike_vis_large = cv2.resize(spike_vis_img, (frame.shape[1], frame.shape[0]), interpolation=cv2.INTER_NEAREST)
        
        # --- D. Affichage combiné ---
        # On colle l'image débruitée et l'image des spikes côte à côte - MODIFIÉ
        combined_view = np.hstack((denoised_frame, spike_vis_large))
        
        cv2.imshow('Gauche: Video | Droite: SNN Spikes en Temps Reel', combined_view)

        # Quitter avec 'q'
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

cap.release()
cv2.destroyAllWindows()