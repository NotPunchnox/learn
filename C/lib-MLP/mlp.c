/*
 * Copyright (c) 2025 NotPunchnox
 */
#include "mlp.h"
#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

// Fonction de perte
double Loss(double prediction, double sortie_attendue) {
    const double epsilon = 1e-15;
    prediction = fmax(epsilon, fmin(1.0 - epsilon, prediction));
    return -(sortie_attendue * log(prediction) + (1.0 - sortie_attendue) * log(1.0 - prediction));
}

// Fonction sigmoïde
double sigmoide(double y) {
    return 1.0 / (1.0 + exp(-y));
}

// Dérivée de la sigmoïde
double sigmoid_derivative(double y) {
    double sig = sigmoide(y);
    return sig * (1.0 - sig);
}

// Crée un réseau neuronal
MLP* mlp_create(int nb_inputs, int nb_layers, const int* nb_neurons_per_layer, int nb_outputs) {
    MLP* nn = (MLP*)malloc(sizeof(MLP));
    nn->nb_inputs = nb_inputs;
    nn->nb_layers = nb_layers;
    nn->nb_outputs = nb_outputs;
    nn->nb_neurons_per_layer = (int*)malloc(nb_layers * sizeof(int));
    nn->weights = (double***)malloc(nb_layers * sizeof(double**));
    nn->biases = (double**)malloc(nb_layers * sizeof(double*));

    for (int layer = 0; layer < nb_layers; layer++) {
        nn->nb_neurons_per_layer[layer] = (layer == nb_layers - 1) ? nb_outputs : nb_neurons_per_layer[layer];
        nn->biases[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
        nn->weights[layer] = (double**)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double*));
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            int nb_prev = (layer == 0 ? nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            nn->weights[layer][neuron] = (double*)malloc(nb_prev * sizeof(double));
        }
    }
    return nn;
}

// Libère la mémoire
void mlp_free(MLP* nn) {
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            free(nn->weights[layer][neuron]);
        }
        free(nn->weights[layer]);
        free(nn->biases[layer]);
    }
    free(nn->weights);
    free(nn->biases);
    free(nn->nb_neurons_per_layer);
    free(nn);
}

// Initialise les poids et biais
void mlp_init_weights(MLP* nn) {
    srand(time(NULL));
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            nn->biases[layer][neuron] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
            int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            for (int input = 0; input < nb_prev; input++) {
                nn->weights[layer][neuron][input] = (double)rand() / RAND_MAX;
            }
        }
    }
}

// Affiche le réseau
void mlp_print_network(MLP* nn) {
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        printf("Layer %d:\n", layer);
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            printf("  Neuron %d:\n", neuron);
            printf("    Bias: %f\n", nn->biases[layer][neuron]);
            printf("    Weights: ");
            int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            for (int input = 0; input < nb_prev; input++) {
                printf("%f ", nn->weights[layer][neuron][input]);
            }
            printf("\n");
        }
    }
}

// Propagation avant
double* mlp_feed_forward(MLP* nn, const int* input, double** layer_inputs, double** layer_output) {
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        layer_inputs[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
        layer_output[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            layer_inputs[layer][neuron] = nn->biases[layer][neuron];
            int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            for (int k = 0; k < nb_prev; k++) {
                if (layer == 0) {
                    layer_inputs[layer][neuron] += input[k] * nn->weights[layer][neuron][k];
                } else {
                    layer_inputs[layer][neuron] += layer_output[layer - 1][k] * nn->weights[layer][neuron][k];
                }
            }
            layer_output[layer][neuron] = sigmoide(layer_inputs[layer][neuron]);
        }
    }

    // Retourne une copie des sorties de la dernière couche
    double* outputs = (double*)malloc(nn->nb_outputs * sizeof(double));
    for (int i = 0; i < nn->nb_outputs; i++) {
        outputs[i] = layer_output[nn->nb_layers - 1][i];
    }
    return outputs;
}

// Entrainement
void mlp_train(MLP* nn, const int (*inputs)[2], const int* targets, int nb_samples, double learning_rate, int max_epochs) {
    int nb_threads = omp_get_max_threads();

    // Allocation mémoire pour la propagation
    double** layer_inputs = (double**)malloc(nn->nb_layers * sizeof(double*));
    double** layer_output = (double**)malloc(nn->nb_layers * sizeof(double*));
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        layer_inputs[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
        layer_output[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
    }

    // Boucle d'entrainement
    for (int epoch = 0; epoch < max_epochs; epoch++) {
        double total_error = 0.0;

        // Allocation pour les gradients par thread
        double**** grad_weights = malloc(nb_threads * sizeof(double***));
        double*** grad_biases = malloc(nb_threads * sizeof(double**));

        for (int t = 0; t < nb_threads; t++) {
            grad_weights[t] = malloc(nn->nb_layers * sizeof(double**));
            grad_biases[t] = malloc(nn->nb_layers * sizeof(double*));
            for (int layer = 0; layer < nn->nb_layers; layer++) {
                grad_weights[t][layer] = malloc(nn->nb_neurons_per_layer[layer] * sizeof(double*));
                grad_biases[t][layer] = malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
                for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                    int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
                    grad_weights[t][layer][neuron] = calloc(nb_prev, sizeof(double));
                    grad_biases[t][layer][neuron] = 0.0;
                }
            }
        }

        #pragma omp parallel for reduction(+:total_error)
        for (int i = 0; i < nb_samples; i++) {
            int tid = omp_get_thread_num();

            // Propagation avant
            double* final_output = mlp_feed_forward(nn, inputs[i], layer_inputs, layer_output);

            // Calcul erreur
            for (int o = 0; o < nn->nb_outputs; o++)
                total_error += Loss(final_output[o], targets[i * nn->nb_outputs + o]);

            // Rétropropagation
            double** delta = (double**)malloc(nn->nb_layers * sizeof(double*));
            for (int layer = 0; layer < nn->nb_layers; layer++) {
                delta[layer] = (double*)calloc(nn->nb_neurons_per_layer[layer], sizeof(double));
            }

            // Delta pour la couche de sortie
            for (int o = 0; o < nn->nb_outputs; o++) {
                delta[nn->nb_layers - 1][o] =
                    (final_output[o] - targets[i * nn->nb_outputs + o]) *
                    sigmoid_derivative(layer_inputs[nn->nb_layers - 1][o]);
            }

            // Delta pour les couches cachées
            for (int layer = nn->nb_layers - 2; layer >= 0; layer--) {
                for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                    for (int next_neuron = 0; next_neuron < nn->nb_neurons_per_layer[layer + 1]; next_neuron++) {
                        delta[layer][neuron] += delta[layer + 1][next_neuron] * nn->weights[layer + 1][next_neuron][neuron];
                    }
                    delta[layer][neuron] *= sigmoid_derivative(layer_inputs[layer][neuron]);
                }
            }

            // Accumulation des gradients dans grad_weights / grad_biases
            for (int layer = 0; layer < nn->nb_layers; layer++) {
                for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                    int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
                    for (int k = 0; k < nb_prev; k++) {
                        grad_weights[tid][layer][neuron][k] += delta[layer][neuron] *
                                                              (layer == 0 ? inputs[i][k] : layer_output[layer - 1][k]);
                    }
                    grad_biases[tid][layer][neuron] += delta[layer][neuron];
                }
            }

            // Libération
            free(final_output);
            for (int layer = 0; layer < nn->nb_layers; layer++) free(delta[layer]);
            free(delta);
        }

        // Mise à jour globale des poids et biais
        for (int layer = 0; layer < nn->nb_layers; layer++) {
            for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
                for (int k = 0; k < nb_prev; k++) {
                    double sum = 0.0;
                    for (int t = 0; t < nb_threads; t++) sum += grad_weights[t][layer][neuron][k];
                    nn->weights[layer][neuron][k] -= learning_rate * sum / nb_samples;
                    free(grad_weights[0][layer][neuron]); // Libération d'un thread (tous identiques en taille)
                }
                double sum_bias = 0.0;
                for (int t = 0; t < nb_threads; t++) sum_bias += grad_biases[t][layer][neuron];
                nn->biases[layer][neuron] -= learning_rate * sum_bias / nb_samples;
            }
            for (int t = 0; t < nb_threads; t++) {
                free(grad_weights[t][layer]);
                free(grad_biases[t][layer]);
            }
        }
        for (int t = 0; t < nb_threads; t++) free(grad_weights[t]);
        for (int t = 0; t < nb_threads; t++) free(grad_biases[t]);
        free(grad_weights);
        free(grad_biases);

        printf("Epoch %d, Erreur moyenne: %f\n", epoch, total_error / (nb_samples * nn->nb_outputs));
    }

    for (int layer = 0; layer < nn->nb_layers; layer++) {
        free(layer_inputs[layer]);
        free(layer_output[layer]);
    }
    free(layer_inputs);
    free(layer_output);
}