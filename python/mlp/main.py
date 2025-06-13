import numpy as np
import random
from math import *

# Fonction de calcul du taux d'erreur ( log-vraisemblance négative )
def Loss(prediction, sortie_attendue):
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
    learning_rate = 0.1
    max_epochs = 5000
    nb_layers = 3
    nb_neurons_per_layers = [3, 3, 1]
    nb_inputs = 2

    # Données d'entrainement ( XOR )
    x = [
        [0, 0],
        [1, 0],
        [0, 1],
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
                        for k in range(len(w[layer][n])):
                            layer_inputs[layer][n] += layer_output[layer-1][k] * w[layer][n][k]

                    layer_output[layer][n] = sigmoide(layer_inputs[layer][n])
            
            final_output = layer_output[nb_layers-1][nb_neurons_per_layers[nb_layers-1]-1]
            
            # Calcul du taux d'erreur
            total_error += Loss(final_output, yy[i])

            # Rétropropagation
            delta = (final_output - yy[i]) * sigmoide_derivative(final_output)
            
            for layer in reversed(range(nb_layers)):
                for n in range(nb_neurons_per_layers[layer]):

                    if layer == nb_layers-1:
                        for k in range(len(w[layer][n])):
                            w[layer][n][k] -= learning_rate * delta * layer_output[layer][n]
                        b[layer][n] -= learning_rate * delta
                    else:
                        delta_hidden = 0
                        for k in range(len(w[layer][n])):
                            delta_hidden += delta * w[layer][n][k] * sigmoide_derivative(layer_inputs[layer][n])
                            w[layer][n][k] -= learning_rate * delta_hidden * layer_output[layer][k]
                        b[layer][n] -= learning_rate * delta_hidden

        # Afficher les stats
        print(f"Epoch: {epoch}, Erreur moyenne: {total_error}")

        for i in range(len(x)):
            # Lister les couches
            for layer in range(nb_layers):
                layer_inputs[layer] = {}
                layer_output[layer] = {}

                # Lister les neurones de la couche ( layer )
                for n in range(nb_neurons_per_layers[layer]):
                    layer_inputs[layer][n] = b[layer][neuron]
                    
                    if layer == 0:
                        for k in range(nb_inputs):
                            layer_inputs[layer][n] += x[i][k] * w[layer][n][k]
                    else:
                        for k in range(len(w[layer][n])):
                            layer_inputs[layer][n] += layer_output[layer-1][n]

                    layer_output[layer][n] = sigmoide(layer_inputs[layer][n])
            
            final_output = layer_output[nb_layers-1][nb_neurons_per_layers[nb_layers-1]-1]
            print(f"Entrée: ({x[i][0]}, {x[i][1]}), Sortie: {final_output}, Attendu: {yy[i]}")

main()


# By NotPunchnox