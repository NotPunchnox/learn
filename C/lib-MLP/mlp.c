#include "mlp.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Fonction de calcul de la perte (log-vraisemblance négative)
static double loss(double prediction, double target) {
  const double epsilon = 1e-15;
  prediction = fmax(epsilon, fmin(1.0 - epsilon, prediction));
  return -(target * log(prediction) + (1.0 - target) * log(1.0 - prediction));
}

// Fonctions d'activation
static double sigmoid(double y) { return 1.0 / (1.0 + exp(-y)); }

static double relu(double y) { return y > 0.0 ? y : 0.0; }

static double tanh_act(double y) { return tanh(y); }

// Dérivées des fonctions d'activation
static double sigmoid_derivative(double y) {
  double sig = sigmoid(y);
  return sig * (1.0 - sig);
}

static double relu_derivative(double y) { return y > 0.0 ? 1.0 : 0.0; }

static double tanh_derivative(double y) {
  double t = tanh(y);
  return 1.0 - t * t;
}

// Création d'une configuration
MLPConfig *mlp_config_create(int nb_layers, const int *nb_neurons,
                             const ActivationType *activations,
                             double learning_rate, int max_epochs,
                             bool verbose) {
  MLPConfig *config = malloc(sizeof(MLPConfig));
  if (!config)
    return NULL;

  config->nb_layers = nb_layers;
  config->nb_neurons = malloc(nb_layers * sizeof(int));
  config->activations = malloc(nb_layers * sizeof(ActivationType));
  if (!config->nb_neurons || !config->activations) {
    free(config->nb_neurons);
    free(config->activations);
    free(config);
    return NULL;
  }

  for (int i = 0; i < nb_layers; i++) {
    config->nb_neurons[i] = nb_neurons[i];
    config->activations[i] = activations ? activations[i] : ACTIVATION_SIGMOID;
  }
  config->learning_rate = learning_rate;
  config->max_epochs = max_epochs;
  config->verbose = verbose;

  return config;
}

// Libération de la configuration
void mlp_config_free(MLPConfig *config) {
  if (!config)
    return;
  free(config->nb_neurons);
  free(config->activations);
  free(config);
}

// Création d'un MLP
MLP *mlp_create_with_config(const MLPConfig *config, int nb_inputs) {
  if (!config || config->nb_layers < 1) {
    fprintf(stderr,
            "Erreur: Configuration non valide ou aucune couche définie\n");
    return NULL;
  }

  MLP *mlp = malloc(sizeof(MLP));
  if (!mlp)
    return NULL;

  mlp->config = *config;
  mlp->nb_layers = config->nb_layers;
  mlp->nb_neurons = malloc(config->nb_layers * sizeof(int));
  mlp->inputs_per_layer = malloc(config->nb_layers * sizeof(int));
  mlp->weights = malloc(config->nb_layers * sizeof(double **));
  mlp->biases = malloc(config->nb_layers * sizeof(double *));
  mlp->activations = malloc(config->nb_layers * sizeof(ActivationType));
  if (!mlp->nb_neurons || !mlp->inputs_per_layer || !mlp->weights ||
      !mlp->biases || !mlp->activations) {
    free(mlp->nb_neurons);
    free(mlp->inputs_per_layer);
    free(mlp->weights);
    free(mlp->biases);
    free(mlp->activations);
    free(mlp);
    return NULL;
  }

  // Initialisation des pointeurs de fonctions
  mlp->activation_funcs[ACTIVATION_SIGMOID] = sigmoid;
  mlp->activation_funcs[ACTIVATION_RELU] = relu;
  mlp->activation_funcs[ACTIVATION_TANH] = tanh_act;
  mlp->activation_derivs[ACTIVATION_SIGMOID] = sigmoid_derivative;
  mlp->activation_derivs[ACTIVATION_RELU] = relu_derivative;
  mlp->activation_derivs[ACTIVATION_TANH] = tanh_derivative;

  mlp->inputs_per_layer[0] = nb_inputs;
  for (int i = 0; i < config->nb_layers; i++) {
    mlp->nb_neurons[i] = config->nb_neurons[i];
    mlp->activations[i] = config->activations[i];
    if (i > 0)
      mlp->inputs_per_layer[i] = config->nb_neurons[i - 1];

    mlp->weights[i] = malloc(config->nb_neurons[i] * sizeof(double *));
    mlp->biases[i] = malloc(config->nb_neurons[i] * sizeof(double));
    if (!mlp->weights[i] || !mlp->biases[i]) {
      for (int j = 0; j <= i; j++) {
        free(mlp->weights[j]);
        free(mlp->biases[j]);
      }
      free(mlp->nb_neurons);
      free(mlp->inputs_per_layer);
      free(mlp->weights);
      free(mlp->biases);
      free(mlp->activations);
      free(mlp);
      return NULL;
    }
    for (int j = 0; j < config->nb_neurons[i]; j++) {
      mlp->weights[i][j] = malloc(mlp->inputs_per_layer[i] * sizeof(double));

      if (!mlp->weights[i][j]) {
        for (int k = 0; k < j; k++)
          free(mlp->weights[i][k]);
        for (int k = 0; k <= i; k++)
          free(mlp->weights[k]), free(mlp->biases[k]);

        free(mlp->nb_neurons);
        free(mlp->inputs_per_layer);
        free(mlp->weights);
        free(mlp->biases);
        free(mlp->activations);
        free(mlp);
        return NULL;
      }

      for (int k = 0; k < mlp->inputs_per_layer[i]; k++) {
        mlp->weights[i][j][k] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
      }
      mlp->biases[i][j] = 0.1;
    }
  }

  return mlp;
}

// Libération de la mémoire
void mlp_free(MLP *mlp) {
  if (!mlp)
    return;
  for (int i = 0; i < mlp->nb_layers; i++) {

    for (int j = 0; j < mlp->nb_neurons[i]; j++)
      free(mlp->weights[i][j]);

    free(mlp->weights[i]);
    free(mlp->biases[i]);
  }
  free(mlp->nb_neurons);
  free(mlp->inputs_per_layer);
  free(mlp->weights);
  free(mlp->biases);
  free(mlp->activations);
  free(mlp);
}

// Prédiction
double mlp_predict(MLP *mlp, const double *input, bool verbose) {
  if (!mlp || mlp->nb_layers < 1) {
    fprintf(stderr, "Erreur: MLP non valide ou aucune couche définie\n");
    return 0.0;
  }

  double **layer_outputs = malloc(mlp->nb_layers * sizeof(double *));
  double **layer_inputs = malloc(mlp->nb_layers * sizeof(double *));

  if (!layer_outputs || !layer_inputs) {
    fprintf(stderr, "Erreur: Échec de l'allocation mémoire\n");
    free(layer_outputs);
    free(layer_inputs);
    return 0.0;
  }

  for (int layer = 0; layer < mlp->nb_layers; layer++) {
    layer_outputs[layer] = malloc(mlp->nb_neurons[layer] * sizeof(double));
    layer_inputs[layer] = malloc(mlp->nb_neurons[layer] * sizeof(double));
    if (!layer_outputs[layer] || !layer_inputs[layer]) {
      fprintf(stderr,
              "Erreur: Échec de l'allocation mémoire pour la couche %d\n",
              layer);
      for (int l = 0; l <= layer; l++) {
        free(layer_outputs[l]);
        free(layer_inputs[l]);
      }
      free(layer_outputs);
      free(layer_inputs);
      return 0.0;
    }
    for (int j = 0; j < mlp->nb_neurons[layer]; j++) {
      layer_inputs[layer][j] = mlp->biases[layer][j];
      for (int k = 0; k < mlp->inputs_per_layer[layer]; k++) {
        layer_inputs[layer][j] +=
            (layer == 0 ? input[k] : layer_outputs[layer - 1][k]) *
            mlp->weights[layer][j][k];
      }
      layer_outputs[layer][j] = mlp->activation_funcs[mlp->activations[layer]](
          layer_inputs[layer][j]);
      if (verbose) {
        printf("Couche %d, Neurone %d: Entrée = %f, Sortie = %f\n", layer, j,
               layer_inputs[layer][j], layer_outputs[layer][j]);
      }
    }
  }

  double output = layer_outputs[mlp->nb_layers - 1][0];
  for (int layer = 0; layer < mlp->nb_layers; layer++) {
    free(layer_outputs[layer]);
    free(layer_inputs[layer]);
  }
  free(layer_outputs);
  free(layer_inputs);
  return output;
}

// Entraînement
void mlp_train(MLP *mlp, const double *inputs, const double *targets,
               int nb_samples, int nb_inputs) {
  if (!mlp || mlp->nb_layers < 1) {
    fprintf(stderr, "Erreur: MLP non valide ou aucune couche définie\n");
    return;
  }

  // Paramètres d'entraînement
  double learning_rate = mlp->config.learning_rate;
  int max_epochs = mlp->config.max_epochs;
  bool verbose = mlp->config.verbose;

  for (int epoch = 0; epoch < max_epochs; epoch++) {
    double total_error = 0.0;
    for (int sample = 0; sample < nb_samples; sample++) {
      const double *input = &inputs[sample * nb_inputs];
      double target = targets[sample];

      // Propagation avant
      double **layer_outputs = malloc(mlp->nb_layers * sizeof(double *));
      double **layer_inputs = malloc(mlp->nb_layers * sizeof(double *));
      for (int layer = 0; layer < mlp->nb_layers; layer++) {
        layer_outputs[layer] = malloc(mlp->nb_neurons[layer] * sizeof(double));
        layer_inputs[layer] = malloc(mlp->nb_neurons[layer] * sizeof(double));
        for (int j = 0; j < mlp->nb_neurons[layer]; j++) {
          layer_inputs[layer][j] = mlp->biases[layer][j];

          for (int k = 0; k < mlp->inputs_per_layer[layer]; k++) {
            layer_inputs[layer][j] +=
                (layer == 0 ? input[k] : layer_outputs[layer - 1][k]) *
                mlp->weights[layer][j][k];
          }
          layer_outputs[layer][j] =
              mlp->activation_funcs[mlp->activations[layer]](
                  layer_inputs[layer][j]);
        }
      }

      // Calcul de la perte
      double output = layer_outputs[mlp->nb_layers - 1][0];
      total_error += loss(output, target);

      // Rétropropagation
      double *deltas =
          malloc(mlp->nb_neurons[mlp->nb_layers - 1] * sizeof(double));
      for (int j = 0; j < mlp->nb_neurons[mlp->nb_layers - 1]; j++) {
        deltas[j] =
            (output - target) *
            mlp->activation_derivs[mlp->activations[mlp->nb_layers - 1]](
                layer_inputs[mlp->nb_layers - 1][j]);
      }

      for (int layer = mlp->nb_layers - 1; layer >= 0; layer--) {
        double *new_deltas =
            malloc(mlp->inputs_per_layer[layer] * sizeof(double));
        for (int j = 0; j < mlp->nb_neurons[layer]; j++) {
          for (int k = 0; k < mlp->inputs_per_layer[layer]; k++) {
            double input_k =
                layer == 0 ? input[k] : layer_outputs[layer - 1][k];
            mlp->weights[layer][j][k] -= learning_rate * deltas[j] * input_k;
          }
          mlp->biases[layer][j] -= learning_rate * deltas[j];

          if (layer > 0) {
            for (int k = 0; k < mlp->inputs_per_layer[layer]; k++) {
              new_deltas[k] = 0.0;
              for (int j2 = 0; j2 < mlp->nb_neurons[layer]; j2++) {
                new_deltas[k] +=
                    deltas[j2] * mlp->weights[layer][j2][k] *
                    mlp->activation_derivs[mlp->activations[layer - 1]](
                        layer_inputs[layer - 1][k]);
              }
            }
          }
        }
        if (layer > 0) {
          free(deltas);
          deltas = new_deltas;
        } else {
          free(new_deltas);
        }
      }
      free(deltas);

      for (int layer = 0; layer < mlp->nb_layers; layer++) {
        free(layer_outputs[layer]);
        free(layer_inputs[layer]);
      }
      free(layer_outputs);
      free(layer_inputs);
    }

    if (verbose && epoch % 1000 == 0) {
      printf("Époque %d, Erreur moyenne: %f\n", epoch,
             total_error / nb_samples);
      mlp_print_weights(mlp);
    }
  }
}

// Affichage des poids et biais
void mlp_print_weights(MLP *mlp) {
  if (!mlp)
    return;

  for (int layer = 0; layer < mlp->nb_layers; layer++) {
    printf("Couche %d :\n", layer);

    for (int neuron = 0; neuron < mlp->nb_neurons[layer]; neuron++) {

      printf("  Neurone %d : ", neuron);

      for (int input = 0; input < mlp->inputs_per_layer[layer]; input++) {
        printf("w[%d][%d][%d] = %f ", layer, neuron, input,
               mlp->weights[layer][neuron][input]);
      }
      printf("b[%d][%d] = %f\n", layer, neuron, mlp->biases[layer][neuron]);
    }
  }
}
