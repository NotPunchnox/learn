import numpy as np
import random
from math import *

# Fonction de calcul du taux d'erreur ( log-vraisemblance négative )
def Loss(prediction, sortie_attendue):
    epsilon = 1e-10
    prediction = max(min(prediction, 1 - epsilon), epsilon)
    return -(sortie_attendue * np.log(prediction) + (1 - sortie_attendue) * np.log(1 - prediction))

# Fonction sigmoïde
def sigmoide(y):
    return 1 / (1 + exp(-y))

# Dérivée de la fonction sigmoïde
def sigmoide_derivative(y):
    sig = sigmoide(y)
    return sig * (1 - sig)


def main():

    # constantes
    learning_rate = 0.2
    max_epochs = 5000
    nb_layers = 2
    nb_neurons_per_layers = [2, 1]
    nb_inputs = 2

    # Données d'entrainement ( XOR )
    x = [
        [0, 0],
        [0, 1],
        [1, 0],
        [1, 1],
    ]
    yy = [0, 1, 1, 0]

    # Initialisation des poids et des biais
    w = {}
    b = {}

    for layer in range(nb_layers):
        b[layer] = {}
        w[layer] = {}

        for neuron in range(nb_neurons_per_layers[layer]):
            b[layer][neuron] = random.uniform(-0.1, 0.1)
            
            if layer == 0:
                w[layer][neuron] = []

                for input in range(nb_inputs):
                    w[layer][neuron].append(random.random())
            
            else:
                w[layer][neuron] = []

                for input in range(nb_neurons_per_layers[layer -1]):
                    w[layer][neuron].append(random.random())

    print(b)
    print(w)

    layer_inputs = {}
    layer_output = {}

    # Boucle d'entrainement
    for epoch in range(max_epochs):
        total_error = 0

        # Lister les instructions de la dataset x
        for i in range(len(x)):

            # Lister les couches
            for layer in range(nb_layers):
                layer_inputs[layer] = {}
                layer_output[layer] = {}

                # Lister les neurones de la couche ( layer )
                for n in range(nb_neurons_per_layers[layer]):
                    layer_inputs[layer][n] = b[layer][n]
                    
                    if layer == 0:
                        for k in range(nb_inputs):
                            layer_inputs[layer][n] += x[i][k] * w[layer][n][k]
                    else:
                        for k in range(nb_neurons_per_layers[layer-1]):
                            layer_inputs[layer][n] += layer_output[layer-1][k] * w[layer][n][k]

                    layer_output[layer][n] = sigmoide(layer_inputs[layer][n])
            
            final_output = layer_output[nb_layers-1][0]
            
            # Calcul du taux d'erreur
            total_error += Loss(final_output, yy[i])

            # Rétropropagation
            deltas = {}
            for layer in range(nb_layers):
                deltas[layer] = {}
                for n in range(nb_neurons_per_layers[layer]):
                    deltas[layer][n] = 0

            deltas[nb_layers-1][0] = (final_output - yy[i]) * sigmoide_derivative(layer_inputs[nb_layers-1][0])

            for layer in reversed(range(nb_layers-1)):
                for n in range(nb_neurons_per_layers[layer]):
                    for next_n in range(nb_neurons_per_layers[layer+1]):
                        deltas[layer][n] += deltas[layer+1][next_n] * w[layer+1][next_n][n]
                    deltas[layer][n] *= sigmoide_derivative(layer_inputs[layer][n])

            for layer in range(nb_layers):
                for n in range(nb_neurons_per_layers[layer]):
                    if layer == 0:
                        for k in range(nb_inputs):
                            w[layer][n][k] -= learning_rate * deltas[layer][n] * x[i][k]
                    else:
                        for k in range(nb_neurons_per_layers[layer-1]):
                            w[layer][n][k] -= learning_rate * deltas[layer][n] * layer_output[layer-1][k]
                    b[layer][n] -= learning_rate * deltas[layer][n]

        # Afficher les stats
        print(f"Epoch: {epoch}, Erreur moyenne: {total_error / len(x)}")

    for i in range(len(x)):
        # Lister les couches
        for layer in range(nb_layers):
            layer_inputs[layer] = {}
            layer_output[layer] = {}

            # Lister les neurones de la couche ( layer )
            for n in range(nb_neurons_per_layers[layer]):
                layer_inputs[layer][n] = b[layer][n]
                
                if layer == 0:
                    for k in range(nb_inputs):
                        layer_inputs[layer][n] += x[i][k] * w[layer][n][k]
                else:
                    for k in range(nb_neurons_per_layers[layer-1]):
                        layer_inputs[layer][n] += layer_output[layer-1][k] * w[layer][n][k]

                layer_output[layer][n] = sigmoide(layer_inputs[layer][n])
        
        final_output = layer_output[nb_layers-1][0]
        print(f"Entrée: ({x[i][0]}, {x[i][1]}), Sortie: {final_output}, Attendu: {yy[i]}")

main()