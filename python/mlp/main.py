import numpy as np
import random
from math import *

# Fonction de calcul du taux d'erreur ( log-vraisemblance négative )
def Loss(prediction, sortie_attendue):
    sortie_attendue = np.array(sortie_attendue)
    prediction = np.array(prediction)

    if sortie_attendue.size > 1 or prediction.size > 1:
        raise ValueError("Les entrées doivent être des scalaires ou des tableaux à un seul élément.")

    sortie_attendue = sortie_attendue.item()
    prediction = prediction.item()
    return (sortie_attendue * np.log(prediction) + (1 - sortie_attendue) * np.log(1 - prediction))

# Fonction sigmoïde
def sigmoide(y):
    return 1 / (1 + exp(-y))

# Dérivée de la fonction sigmoïde
def sigmoide_derivative(y):
    sig = sigmoide(y)
    sig * (1 - sig)



def main():

    # constantes
    learning_rate = 0.1
    max_epochs = 500
    nb_layers = 3
    nb_neurons_per_layers = [3, 3, 1]
    nb_inputs = 2

    # Données d'entrainement ( XOR )
    x = [
        [0, 0],
        [1, 0],
        [0, 1],
        [0, 0],
    ]
    yy = [0, 1, 1, 0]

    # Initialisation des poids et des biais
    w = {}
    b = {}

    for layer in range(nb_layers):
        b[layer] = {}
        w[layer] = {}

        for neuron in range(nb_neurons_per_layers[layer]):
            b[layer][neuron] = 1
            
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
                    layer_inputs[layer][n] = b[layer][neuron]
                    
                    if layer == 0:
                        for k in range(nb_inputs):
                            layer_inputs[layer][n] += x[i][k] * w[layer][n][k]
                    
                    layer_output[layer][n] = sigmoide(layer_inputs[layer][n])
                    


main()