import random
import time
import os

# L'ENVIRONNEMENT
GRILLE = [
    ['S', '.', '.', 'X', '.'],
    ['.', 'X', '.', 'X', '.'],
    ['.', 'X', '.', '.', '.'],
    ['.', '.', '.', 'X', 'X'],
    ['X', 'X', '.', '.', 'T']
]

LIGNES = len(GRILLE)
COLS = len(GRILLE[0])

# Actions : 0=Haut, 1=Bas, 2=Gauche, 3=Droite
ACTIONS = [0, 1, 2, 3]
SYMBOLES_ACTIONS = {0: '↑', 1: '↓', 2: '←', 3: '→'}

# Trouver la position de départ
for i in range(LIGNES):
    for j in range(COLS):
        if GRILLE[i][j] == 'S':
            DEPART = (i, j)

# PARAMÈTRES DU Q-LEARNING
alpha = 0.1
gamma = 0.9
epsilon = 0.2
episodes = 500

# Q(i,j) : (ligne, colonne) -> Valeur : Liste des 4 valeurs Q [Haut, Bas, Gauche, Droite]
Q = {}
for i in range(LIGNES):
    for j in range(COLS):
        Q[(i, j)] = [0.0, 0.0, 0.0, 0.0]

# FONCTION DU SIMULATEUR
def faire_un_pas(etat, action):
    l, c = etat
    # Calcul de la nouvelle position dans la matrice
    if action == 0: l -= 1   # Haut
    elif action == 1: l += 1 # Bas
    elif action == 2: c -= 1 # Gauche
    elif action == 3: c += 1 # Droite

    # Vérification des limites et des obstacles
    if l < 0 or l >= LIGNES or c < 0 or c >= COLS or GRILLE[l][c] == 'X':
        return etat, -5, False
    
    # Si pas d'obstacle, faire bouger l'agent
    nouvel_etat = (l, c)
    
    # Objectif atteint ?
    if GRILLE[l][c] == 'T':
        return nouvel_etat, 20, True
    else:
        return nouvel_etat, -1, False

# ENTRAÎNEMENT
print("Entraînement en cours (500 épisodes)...")
for episode in range(episodes):
    etat = DEPART
    termine = False
    
    while not termine:
        # Epsilon-Greedy
        if random.uniform(0, 1) < epsilon:
            action = random.choice(ACTIONS)
        else:
            action = Q[etat].index(max(Q[etat]))

        # Interaction avec l'environnement
        prochain_etat, recompense, termine = faire_un_pas(etat, action)

        # Equation de Bellman
        max_q_prochain = max(Q[prochain_etat])
        Q[etat][action] = Q[etat][action] + alpha * (recompense + gamma * max_q_prochain - Q[etat][action])

        etat = prochain_etat

#  VISUALISATION DU MEILLEURE TRAJET APPRIS
for i in range(LIGNES):
    ligne_visuelle = ""

    for j in range(COLS):
        if GRILLE[i][j] == 'T':
            ligne_visuelle += " T "
        elif GRILLE[i][j] == 'X':
            ligne_visuelle += " X "
        else:
            # Chercher la meilleure action pour cette case
            meilleure_action = Q[(i, j)].index(max(Q[(i, j)]))
            # Vérifier si l'agent a vraiment exploré la case
            if max(Q[(i, j)]) == 0.0 and min(Q[(i, j)]) == 0.0:
                ligne_visuelle += " . " 
            else:
                ligne_visuelle += f" {SYMBOLES_ACTIONS[meilleure_action]} "
    print(ligne_visuelle)