/*
 * Copyright (c) 2025 NotPunchnox
 */
#include "mlp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Fonction de perte (log-vraisemblance négative, conservée pour compatibilité)
double Loss(double prediction, double sortie_attendue) {
    const double epsilon = 1e-15;
    prediction = fmax(epsilon, fmin(1.0 - epsilon, prediction));
    return -(sortie_attendue * log(prediction) + (1.0 - sortie_attendue) * log(1.0 - prediction));
}

// Entropie croisée pour classification multiclasse
double cross_entropy_loss(double* predictions, int* target, int size) {
    double loss = 0.0;
    const double epsilon = 1e-15;
    for (int i = 0; i < size; i++) {
        double pred = fmax(epsilon, fmin(1.0 - epsilon, predictions[i]));
        loss -= target[i] * log(pred);
    }
    return loss;
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

// Fonction ReLU
double relu(double x) {
    return x > 0.0 ? x : 0.0;
}

// Dérivée de ReLU
double relu_derivative(double x) {
    return x > 0.0 ? 1.0 : 0.0;
}

// Fonction softmax
void softmax(double* input, double* output, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        output[i] = exp(input[i]);
        sum += output[i];
    }
    for (int i = 0; i < size; i++) {
        output[i] /= sum;
    }
}

// Charge les données MNIST depuis un fichier CSV
void load_dataset(const char* filename, double*** inputs, int** labels, int* nb_samples) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : impossible d'ouvrir %s\n", filename);
        exit(1);
    }

    // Compter le nombre d'échantillons
    *nb_samples = 0;
    char line[5000];
    while (fgets(line, sizeof(line), file)) {
        (*nb_samples)++;
    }
    fseek(file, 0, SEEK_SET);

    // Allouer la mémoire
    *inputs = (double**)malloc(*nb_samples * sizeof(double*));
    *labels = (int*)malloc(*nb_samples * 10 * sizeof(int)); // One-hot encoding

    // Lire les données
    int idx = 0;
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        int label = atoi(token);
        (*inputs)[idx] = (double*)malloc(784 * sizeof(double));
        for (int i = 0; i < 10; i++) {
            (*labels)[idx * 10 + i] = (i == label) ? 1 : 0; // One-hot encoding
        }
        for (int i = 0; i < 784; i++) {
            token = strtok(NULL, ",");
            (*inputs)[idx][i] = atoi(token) / 255.0; // Normalisation
        }
        idx++;
    }
    fclose(file);
}

// Crée un réseau neuronal
MLP* mlp_create(int nb_inputs, int nb_layers, const int* nb_neurons_per_layer, int nb_outputs, const int* activation_types) {
    MLP* nn = (MLP*)malloc(sizeof(MLP));
    nn->nb_inputs = nb_inputs;
    nn->nb_layers = nb_layers;
    nn->nb_outputs = nb_outputs;
    nn->nb_neurons_per_layer = (int*)malloc(nb_layers * sizeof(int));
    nn->weights = (double***)malloc(nb_layers * sizeof(double**));
    nn->biases = (double**)malloc(nb_layers * sizeof(double*));
    nn->activation_types = (int*)malloc(nb_layers * sizeof(int));

    for (int layer = 0; layer < nb_layers; layer++) {
        nn->nb_neurons_per_layer[layer] = (layer == nb_layers - 1) ? nb_outputs : nb_neurons_per_layer[layer];
        nn->biases[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
        nn->weights[layer] = (double**)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double*));
        nn->activation_types[layer] = activation_types[layer];
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
    free(nn->activation_types);
    free(nn);
}


// Sauvegarde le modèle dans un fichier binaire
void mlp_save_model(MLP* nn, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur : impossible d'ouvrir %s pour écriture\n", filename);
        exit(1);
    }

    // Sauvegarde des métadonnées
    fwrite(&nn->nb_inputs, sizeof(int), 1, file);
    fwrite(&nn->nb_layers, sizeof(int), 1, file);
    fwrite(&nn->nb_outputs, sizeof(int), 1, file);
    fwrite(nn->nb_neurons_per_layer, sizeof(int), nn->nb_layers, file);
    fwrite(nn->activation_types, sizeof(int), nn->nb_layers, file);

    // Sauvegarde des poids et biais
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        fwrite(nn->biases[layer], sizeof(double), nn->nb_neurons_per_layer[layer], file);
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            fwrite(nn->weights[layer][neuron], sizeof(double), nb_prev, file);
        }
    }

    fclose(file);
}

// Charge un modèle depuis un fichier binaire
MLP* mlp_load_model(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur : impossible d'ouvrir %s pour lecture\n", filename);
        exit(1);
    }

    // Lecture des métadonnées
    int nb_inputs, nb_layers, nb_outputs;
    fread(&nb_inputs, sizeof(int), 1, file);
    fread(&nb_layers, sizeof(int), 1, file);
    fread(&nb_outputs, sizeof(int), 1, file);

    int* nb_neurons_per_layer = (int*)malloc(nb_layers * sizeof(int));
    int* activation_types = (int*)malloc(nb_layers * sizeof(int));
    fread(nb_neurons_per_layer, sizeof(int), nb_layers, file);
    fread(activation_types, sizeof(int), nb_layers, file);

    // Création du réseau
    MLP* nn = mlp_create(nb_inputs, nb_layers, nb_neurons_per_layer, nb_outputs, activation_types);

    // Lecture des poids et biais
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        fread(nn->biases[layer], sizeof(double), nn->nb_neurons_per_layer[layer], file);
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            fread(nn->weights[layer][neuron], sizeof(double), nb_prev, file);
        }
    }

    fclose(file);
    free(nb_neurons_per_layer); // Libéré après mlp_create
    free(activation_types);
    return nn;
}

// Initialise les poids et biais
void mlp_init_weights(MLP* nn) {
    srand(time(NULL));
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
            nn->biases[layer][neuron] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
            int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
            for (int input = 0; input < nb_prev; input++) {
                nn->weights[layer][neuron][input] = ((double)rand() / RAND_MAX) * 1.0 - 0.5; // Plage [-0.5, 0.5]
            }
        }
    }
}

// Affiche le réseau
void mlp_print_network(MLP* nn) {
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        printf("Layer %d (Activation: %s):\n", layer,
               nn->activation_types[layer] == ACTIVATION_SIGMOID ? "Sigmoid" :
               nn->activation_types[layer] == ACTIVATION_RELU ? "ReLU" : "Softmax");
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
double* mlp_feed_forward(MLP* nn, const double* input, double** layer_inputs, double** layer_output) {
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
            if (nn->activation_types[layer] == ACTIVATION_SIGMOID) {
                layer_output[layer][neuron] = sigmoide(layer_inputs[layer][neuron]);
            } else if (nn->activation_types[layer] == ACTIVATION_RELU) {
                layer_output[layer][neuron] = relu(layer_inputs[layer][neuron]);
            } else if (nn->activation_types[layer] == ACTIVATION_SOFTMAX && layer == nn->nb_layers - 1) {
                // Softmax appliqué à la dernière couche
                softmax(layer_inputs[layer], layer_output[layer], nn->nb_neurons_per_layer[layer]);
                break; // Sortie après softmax
            }
        }
    }

    // Retourne une copie des sorties de la dernière couche
    double* outputs = (double*)malloc(nn->nb_outputs * sizeof(double));
    for (int i = 0; i < nn->nb_outputs; i++) {
        outputs[i] = layer_output[nn->nb_layers - 1][i];
    }
    return outputs;
}

// Entraînement
void mlp_train(MLP* nn, const double** inputs, int* targets, int nb_samples, double learning_rate, int max_epochs, int print_process) {
    double** layer_inputs = (double**)malloc(nn->nb_layers * sizeof(double*));
    double** layer_output = (double**)malloc(nn->nb_layers * sizeof(double*));
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        layer_inputs[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
        layer_output[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
    }

    for (int epoch = 0; epoch < max_epochs; epoch++) {
        double total_error = 0.0;
        for (int i = 0; i < nb_samples; i++) {
            // Propagation avant
            double* final_output = mlp_feed_forward(nn, inputs[i], layer_inputs, layer_output);

            // Calcul de la perte
            total_error += cross_entropy_loss(final_output, &targets[i * nn->nb_outputs], nn->nb_outputs);

            if (print_process == 1 && max_epochs % 100 == 0) {
                printf("Loss: %f\n", cross_entropy_loss(final_output, &targets[i * nn->nb_outputs], nn->nb_outputs));
            }

            // Rétropropagation
            double** delta = (double**)malloc(nn->nb_layers * sizeof(double*));
            for (int layer = 0; layer < nn->nb_layers; layer++) {
                delta[layer] = (double*)malloc(nn->nb_neurons_per_layer[layer] * sizeof(double));
                for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                    delta[layer][neuron] = 0;
                }
            }

            // Delta pour la couche de sortie (softmax + entropie croisée)
            if (nn->activation_types[nn->nb_layers - 1] == ACTIVATION_SOFTMAX) {
                for (int o = 0; o < nn->nb_outputs; o++) {
                    delta[nn->nb_layers - 1][o] = final_output[o] - targets[i * nn->nb_outputs + o];
                }
            } else {
                for (int o = 0; o < nn->nb_outputs; o++) {
                    delta[nn->nb_layers - 1][o] = (final_output[o] - targets[i * nn->nb_outputs + o]) * sigmoid_derivative(layer_inputs[nn->nb_layers - 1][o]);
                }
            }

            // Delta pour les couches cachées
            for (int layer = nn->nb_layers - 2; layer >= 0; layer--) {
                for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                    for (int next_neuron = 0; next_neuron < nn->nb_neurons_per_layer[layer + 1]; next_neuron++) {
                        delta[layer][neuron] += delta[layer + 1][next_neuron] * nn->weights[layer + 1][next_neuron][neuron];
                    }
                    if (nn->activation_types[layer] == ACTIVATION_SIGMOID) {
                        delta[layer][neuron] *= sigmoid_derivative(layer_inputs[layer][neuron]);
                    } else if (nn->activation_types[layer] == ACTIVATION_RELU) {
                        delta[layer][neuron] *= relu_derivative(layer_inputs[layer][neuron]);
                    }
                }
            }

            // Mise à jour des poids et biais
            for (int layer = 0; layer < nn->nb_layers; layer++) {
                for (int neuron = 0; neuron < nn->nb_neurons_per_layer[layer]; neuron++) {
                    int nb_prev = (layer == 0 ? nn->nb_inputs : nn->nb_neurons_per_layer[layer - 1]);
                    for (int k = 0; k < nb_prev; k++) {
                        nn->weights[layer][neuron][k] -= learning_rate * delta[layer][neuron] * (layer == 0 ? inputs[i][k] : layer_output[layer - 1][k]);
                    }
                    nn->biases[layer][neuron] -= learning_rate * delta[layer][neuron];
                }
            }

            // Libère la mémoire temporaire
            free(final_output);
            for (int layer = 0; layer < nn->nb_layers; layer++) {
                free(delta[layer]);
            }
            free(delta);
        }
        if (epoch % 100 == 0) {
            printf("Epoch %d, Erreur moyenne: %f\n", epoch, total_error / nb_samples);
        }
    }

    // Libère la mémoire temporaire
    for (int layer = 0; layer < nn->nb_layers; layer++) {
        free(layer_inputs[layer]);
        free(layer_output[layer]);
    }
    free(layer_inputs);
    free(layer_output);
}