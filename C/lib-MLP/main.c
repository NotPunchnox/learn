/*
 * Copyright (c) 2025 NotPunchnox
 */
#include "mlp.h"
#include <stdio.h>
#include <time.h>

int main() {
    srand(time(NULL));

    // Jeu de données XOR
    const int x[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const int yy[4] = {0, 1, 1, 0};

    // Configuration du réseau
    int nb_neurons_per_layer[2] = {2, 1};
    MLP* nn = mlp_create(2, 2, nb_neurons_per_layer, 1);

    // Initialisation des poids et biais
    mlp_init_weights(nn);

    // Afficher le réseau
    printf("Réseau neuronal initial :\n");
    mlp_print_network(nn);

    // Entraînement
    mlp_train(nn, x, yy, 4, 0.2, 5000);

    // Test final
    printf("\nRésultats après entraînement :\n");
    double** layer_inputs = (double**)malloc(nn->nb_layers * sizeof(double*));
    double** layer_output = (double**)malloc(nn->nb_layers * sizeof(double*));

    for (int i = 0; i < 4; i++) {
        double* final_output = mlp_feed_forward(nn, x[i], layer_inputs, layer_output);
        printf("Entrées: (%d, %d) Attendue: %d Sortie: %f\n", x[i][0], x[i][1], yy[i], final_output[0]);
        free(final_output);
        for (int layer = 0; layer < nn->nb_layers; layer++) {
            free(layer_inputs[layer]);
            free(layer_output[layer]);
        }
    }
    free(layer_inputs);
    free(layer_output);

    // Libération du réseau
    mlp_free(nn);

    return 0;
}