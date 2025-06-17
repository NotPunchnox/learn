/*
 * Copyright (c) 2025 NotPunchnox
 */
#ifndef MLP_H
#define MLP_H

#include <stdlib.h>

// Types d'activation
#define ACTIVATION_SIGMOID 0
#define ACTIVATION_RELU 1
#define ACTIVATION_SOFTMAX 2

// Structure pour représenter un réseau neuronal
typedef struct {
    int nb_layers;              // Nombre de couches
    int nb_inputs;              // Nombre d'entrées
    int* nb_neurons_per_layer;  // Tableau du nombre de neurones par couche
    double*** weights;          // Poids : weights[layer][neuron][input]
    double** biases;            // Biais : biases[layer][neuron]
    int nb_outputs;             // Nombre de sorties
    int* activation_types;      // Types d'activation par couche
} MLP;

// Prototypes des fonctions publiques
MLP* mlp_create(int nb_inputs, int nb_layers, const int* nb_neurons_per_layer, int nb_outputs, const int* activation_types);

void mlp_free(MLP* nn);

void mlp_init_weights(MLP* nn);

double* mlp_feed_forward(MLP* nn, const double* input, double** layer_inputs, double** layer_output);

void mlp_train(MLP* nn, const double** inputs, int* targets, int nb_samples, double learning_rate, int max_epochs, int print_process);

void mlp_print_network(MLP* nn);

void load_dataset(const char* filename, double*** inputs, int** labels, int* nb_samples);

double Loss(double prediction, double sortie_attendue);


double cross_entropy_loss(double* predictions, int* target, int size);
double sigmoide(double y);
double sigmoid_derivative(double y);
double relu(double x);
double relu_derivative(double x);
void softmax(double* input, double* output, int size);

void mlp_save_model(MLP* nn, const char* filename);
MLP* mlp_load_model(const char* filename);

#endif // MLP_H