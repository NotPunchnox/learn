import torch
import torch.nn as nn
import torch.optim as optim
import random

# Labyrinthe (Environnement)
GRILLE = [
    ['S', '.', '.', 'X', '.'],
    ['.', 'X', '.', 'X', '.'],
    ['.', 'X', '.', '.', '.'],
    ['.', '.', '.', 'X', 'X'],
    ['X', 'X', '.', '.', 'T']
]

LIGNES, COLS = len(GRILLE), len(GRILLE[0])
ACTIONS = [0, 1, 2, 3] # 0:Haut, 1:Bas, 2:Gauche, 3:Droite
SYMBOLES_ACTIONS = {0: '↑', 1: '↓', 2: '←', 3: '→'}

for i in range(LIGNES):
    for j in range(COLS):
        if GRILLE[i][j] == 'S': DEPART = (i, j)

# Convertir un état (ligne, col) en Tenseur PyTorch normalisé (entre 0 et 1)
def etat_vers_tenseur(etat):
    l, c = etat
    return torch.tensor([l / (LIGNES - 1), c / (COLS - 1)], dtype=torch.float32)

def faire_un_pas(etat, action):
    l, c = etat
    if action == 0: l -= 1
    elif action == 1: l += 1
    elif action == 2: c -= 1
    elif action == 3: c += 1

    # Détection des murs et des limites
    if l < 0 or l >= LIGNES or c < 0 or c >= COLS or GRILLE[l][c] == 'X':
        return etat, -5, False
    
    # Si pas d'obstacle, faire bouger l'agent
    nouvel_etat = (l, c)

    # Objectif atteint ? Victoire = +20, Pénalité de temps = -1
    if GRILLE[l][c] == 'T':
        return nouvel_etat, 20, True
    return nouvel_etat, -1, False

# Réseau de neurones
class ReseauQ(nn.Module):
    def __init__(self):
        super().__init__()
        # Entrée : 2 valeurs (coordonnées X et Y)
        # Couche cachée : 32 neurones
        # Sortie : 4 valeurs (scores pour Haut, Bas, Gauche, Droite)
        self.reseau = nn.Sequential(
            nn.Linear(2, 32),
            nn.ReLU(),
            nn.Linear(32, 32),
            nn.ReLU(),
            nn.Linear(32, 4)
        )

    def forward(self, x):
        return self.reseau(x)

modele = ReseauQ()
optimiseur = optim.Adam(modele.parameters(), lr=0.005)
loss_function = nn.MSELoss()

# Sauvegarder les intéractions pour l'apprentissage
memory = []
TAILLE_MAX_MEMOIRE = 2000
TAILLE_LOT = 64

# Paramètres d'apprentissage
gamma = 0.95
epsilon = 1.0
epsilon_min = 0.05
declin_epsilon = 0.995
episodes = 800

print("Entraînement du réseau de neurones en cours...")

# Boucle principale d'entraînement
for episode in range(episodes):
    etat = DEPART
    finish = False
    
    while not finish:
        # Choix de l'action
        if random.uniform(0, 1) < epsilon:
            action = random.choice(ACTIONS)
        else:
            with torch.no_grad():
                scores = modele(etat_vers_tenseur(etat))
                action = torch.argmax(scores).item()

        # Simulation
        next_state, reward, finish = faire_un_pas(etat, action)

        # Stockage dans la mémoire
        memory.append((etat, action, reward, next_state, finish))
        if len(memory) > TAILLE_MAX_MEMOIRE:
            memory.pop(0) # On oublie les souvenirs les plus vieux

        # Apprentissage à partir d'un lot d'expériences
        if len(memory) >= TAILLE_LOT:
            # On pioche un lot au hasard
            lot = random.sample(memory, TAILLE_LOT)
            
            # Préparer les données pour PyTorch
            states_batch = torch.stack([etat_vers_tenseur(s[0]) for s in lot])
            actions_batch = torch.tensor([s[1] for s in lot])
            rewards_batch = torch.tensor([s[2] for s in lot], dtype=torch.float32)
            next_states_batch = torch.stack([etat_vers_tenseur(s[3]) for s in lot])
            done_batch = torch.tensor([s[4] for s in lot], dtype=torch.float32)

            # Les prédictions actuelles du réseau
            current_q_values = modele(states_batch)
            predictions = current_q_values[range(TAILLE_LOT), actions_batch]

            # Le calcul des cibles (Équation de Bellman)
            with torch.no_grad():
                next_q_values = modele(next_states_batch)
                max_q_next = torch.max(next_q_values, dim=1)[0]
                
            cibles = rewards_batch + gamma * max_q_next * (1 - done_batch)

            # 3. Optimisation
            loss = loss_function(predictions, cibles)
            optimiseur.zero_grad()
            loss.backward()
            optimiseur.step()

        etat = next_state

    # Réduction de l'exploration au fil du temps
    epsilon = max(epsilon_min, epsilon * declin_epsilon)

# Affichage de la politique apprise
print("\n--- POLITIQUE OPTIMALE (Apprise par le Réseau de Neurones) ---")
with torch.no_grad():
    for i in range(LIGNES):
        ligne_visuelle = ""
        for j in range(COLS):
            if GRILLE[i][j] == 'T':
                ligne_visuelle += " T "
            elif GRILLE[i][j] == 'X':
                ligne_visuelle += " X "
            else:
                etat = (i, j)
                scores = modele(etat_vers_tenseur(etat))
                meilleure_action = torch.argmax(scores).item()
                ligne_visuelle += f" {SYMBOLES_ACTIONS[meilleure_action]} "
        print(ligne_visuelle)