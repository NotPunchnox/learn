/*
 * Copyright (c) 2025 NotPunchnox
 */
#ifndef MLP_H
#define MLP_H

#include <stdlib.h>

// Structure pour représenter un réseau neuronal
typedef struct {
    int nb_layers;              // Nombre de couches
    int nb_inputs;              // Nombre d'entrées
    int* nb_neurons_per_layer;  // Tableau du nombre de neurones par couche
    double*** weights;          // Poids : weights[layer][neuron][input]
    double** biases;            // Biais : biases[layer][neuron]
    int nb_outputs;             // Nombre de sorties (déterminé par les données)
} MLP;

// Prototypes des fonctions publiques

// Crée un réseau neuronal
MLP* mlp_create(int nb_inputs, int nb_layers, const int* nb_neurons_per_layer, int nb_outputs);

// Libère la mémoire du réseau
void mlp_free(MLP* nn);

// Initialise les poids et biais
void mlp_init_weights(MLP* nn);

// Effectue une propagation avant
double* mlp_feed_forward(MLP* nn, const int* input, double** layer_inputs, double** layer_output);

// Entraîne le réseau
void mlp_train(MLP* nn, const int (*inputs)[2], const int* targets, int nb_samples, double learning_rate, int max_epochs);

// Affiche le réseau
void mlp_print_network(MLP* nn);

// Fonction de perte
double Loss(double prediction, double sortie_attendue);

// Fonction sigmoïde
double sigmoide(double y);

// Dérivée de la sigmoïde
double sigmoid_derivative(double y);

#endif // MLP_H