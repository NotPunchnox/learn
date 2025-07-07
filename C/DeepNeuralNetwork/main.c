/*
 * Copyright (c) 2025 NotPunchnox
 */
#include "lib/mlp.h"
#include <stdio.h>
#include <time.h>

int main() {
    srand(time(NULL));

    // Charger les données MNIST
    double** inputs;
    int* labels;
    int nb_samples;
    load_dataset("mnist_train.csv", &inputs, &labels, &nb_samples);

    // Configuration du réseau
    int nb_neurons_per_layer[3] = {256, 128, 10}; // 3 couches
    int activation_types[3] = {ACTIVATION_RELU, ACTIVATION_RELU, ACTIVATION_SOFTMAX};
    MLP* nn = mlp_create(784, 3, nb_neurons_per_layer, 10, activation_types);

    // Initialisation des poids
    mlp_init_weights(nn);

    // Entraînement
    mlp_train(nn, (const double**)inputs, labels, nb_samples, 0.01, 1, 1);

    // Test final
    printf("\nRésultats après entraînement :\n");
    double** layer_inputs = (double**)malloc(nn->nb_layers * sizeof(double*));
    double** layer_output = (double**)malloc(nn->nb_layers * sizeof(double*));
    int correct = 0;
    for (int i = 0; i < nb_samples; i++) {
        double* output = mlp_feed_forward(nn, inputs[i], layer_inputs, layer_output);
        int predicted = 0;
        double max_prob = output[0];
        for (int j = 1; j < nn->nb_outputs; j++) {
            if (output[j] > max_prob) {
                max_prob = output[j];
                predicted = j;
            }
        }
        int true_label = 0;
        for (int j = 0; j < 10; j++) {
            if (labels[i * 10 + j] == 1) {
                true_label = j;
                break;
            }
        }
        if (predicted == true_label) {
            correct++;
        }
        printf("Échantillon %d: Prédit %d, Attendu %d\n", i, predicted, true_label);
        free(output);
        for (int layer = 0; layer < nn->nb_layers; layer++) {
            free(layer_inputs[layer]);
            free(layer_output[layer]);
        }
    }
    printf("Précision: %.2f%%\n", (double)correct / nb_samples * 100);


    printf("Sauvegarde du model dans: /out/model");
    mlp_save_model(nn, "mnist_model.bin");

    // Libération de la mémoire
    for (int i = 0; i < nb_samples; i++) {
        free(inputs[i]);
    }
    free(inputs);
    free(labels);
    free(layer_inputs);
    free(layer_output);
    mlp_free(nn);

    return 0;
}