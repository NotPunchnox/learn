/*
* Copyright (c)
* 2025 NotPunchnox
*/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define learning_rate 0.2
#define max_epochs 5000
#define nb_layers 2
#define nb_inputs 2

const int nb_neurons_per_layer[nb_layers] = {2, 1};

// Jeu de données
const int x[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1},
};
const int yy[4] = {0, 1, 1, 0};


// Fonction de calcul de la perte (log-vraisemblance négative)
double Loss(double prediction, double sortie_attendue) {
  const double epsilon = 1e-15;
  prediction = fmax(epsilon, fmin(1.0 - epsilon, prediction));
  return -(sortie_attendue * log(prediction) + (1.0 - sortie_attendue) * log(1.0 - prediction));
}

// Fonction d'activation sigmoïde
double sigmoide(double y) { return 1.0 / (1.0 + exp(-y)); }

// Dérivée de la fonction sigmoïde
double sigmoid_derivative(double y) {
  double sig = sigmoide(y);
  return sig * (1.0 - sig);
}

void PrintNeuralNetwork(double ***w, double **b) {
  // Afficher les neurons et biais en fonction des couches
  for (int layer = 0; layer < nb_layers; layer++) {
    printf("Layer %d:\n", layer);
    for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
      printf("  Neuron %d:\n", neuron);
      printf("    Bias: %f\n", b[layer][neuron]);
      printf("    Weights: ");
      for (int input = 0; input < (layer == 0 ? nb_inputs : nb_neurons_per_layer[layer - 1]); input++) {
        printf("%f ", w[layer][neuron][input]);
      }
      printf("\n");
    }
  }
}

void InitWeights(double ***w, double **b) {
  // Lister les couches
  for (int layer = 0; layer < nb_layers; layer++) {

    b[layer] = (double *)malloc(nb_neurons_per_layer[layer] * sizeof(double));
    w[layer] = (double **)malloc(nb_neurons_per_layer[layer] * sizeof(double *));

    // Lister les neurones présent dans la couche et leurs attitrer un biais
    for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
      b[layer][neuron] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
      w[layer][neuron] = (double *)malloc((layer == 0 ? nb_inputs : nb_neurons_per_layer[layer - 1]) * sizeof(double));

      // Lister le nombre d'entrées
      for (int input = 0; input < (layer == 0 ? nb_inputs : nb_neurons_per_layer[layer - 1]); input++) {
        w[layer][neuron][input] = (double)rand() / RAND_MAX;
      }
    }
  };
}

double feedFoward(double **layer_inputs, double **layer_output, double **b, double ***w, int i) {
  // Lister les couches du réseau
      for (int layer = 0; layer < nb_layers; layer++) {
        layer_inputs[layer] = (double *)malloc(nb_neurons_per_layer[layer] * sizeof(double));
        layer_output[layer] = (double *)malloc(nb_neurons_per_layer[layer] * sizeof(double));

        for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
          // Ajouter le biais
          layer_inputs[layer][neuron] = b[layer][neuron];

          // Lister les sorties de la couche précédente ou les entrées si on est
          // sur la première couche
          for (int k = 0; k < (layer == 0 ? nb_inputs : nb_neurons_per_layer[layer - 1]); k++) {

            // Rajouter au biais la somme des entrées et des poids
            if (layer == 0) {
              layer_inputs[layer][neuron] += x[i][k] * w[layer][neuron][k];
            } else {
              layer_inputs[layer][neuron] += layer_output[layer - 1][k] * w[layer][neuron][k];
            }
          }

          // Passer la sortie dans la fonction d'activation (sigmoïde)
          layer_output[layer][neuron] = sigmoide(layer_inputs[layer][neuron]);
        }
      }

      // sortie finale ( dernière couche )
      return layer_output[nb_layers - 1][0];
}


int main() {
  srand(time(NULL));

  // Définir les poids et biais dans la mémoire afin d'avoir des tableaux
  // dynamiques
  double **b = (double **)malloc(nb_layers * sizeof(double *));
  double ***w = (double ***)malloc(nb_layers * sizeof(double **));
  double **layer_inputs = (double **)malloc(nb_layers * sizeof(double *));
  double **layer_output = (double **)malloc(nb_layers * sizeof(double *));

  // Initialisation des poids et des biais
  InitWeights(w, b);

  // Afficher les poids et biais
  PrintNeuralNetwork(w, b);

  // Boucle d'entrainement
  for (int epoch = 0; epoch < max_epochs; epoch++) {

    for (int i = 0; i < sizeof(x) / sizeof(x)[0]; i++) {
      double total_error;


      double final_output = feedFoward(layer_inputs, layer_output, b, w, i);

      // Calcul du taux d'erreur
      total_error += Loss(final_output, yy[i]);
      printf("Loss: %f\n", Loss(final_output, yy[i]));


      // Rétropropagation
      double **delta = (double **)malloc(nb_layers * sizeof(double *));

      for (int layer = 0; layer < nb_layers; layer++) {
        delta[layer] = (double *)malloc(nb_neurons_per_layer[layer] * sizeof(double));
        for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
          delta[layer][neuron] = 0;
        }
      }

      delta[nb_layers - 1][0] = (final_output - yy[i]) * sigmoid_derivative(layer_inputs[nb_layers - 1][0]);

      // Lister les couches à l'envers
      for (int layer = nb_layers - 2; layer >= 0; layer--) {
        for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
          for (int next_neuron = 0; next_neuron < nb_neurons_per_layer[layer + 1]; next_neuron++) {
            delta[layer][neuron] += delta[layer + 1][next_neuron] * w[layer + 1][next_neuron][neuron];
          }
          delta[layer][neuron] *= sigmoid_derivative(layer_inputs[layer][neuron]);
        }
      }

      for (int layer = 0; layer < nb_layers; layer++) {
        for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
          for (int k = 0; k < (layer == 0 ? nb_inputs : nb_neurons_per_layer[layer - 1]); k++) {
            w[layer][neuron][k] -=learning_rate * delta[layer][neuron] * (layer == 0 ? x[i][k] : layer_output[layer - 1][k]);
          }

          b[layer][neuron] -= learning_rate * delta[layer][neuron];
        }
      }
      free(delta);
    }
  }

  // Test final:
  for (int i = 0; i < sizeof(x) / sizeof(x)[0]; i++) {

    double final_output = feedFoward(layer_inputs, layer_output, b, w, i);
    printf("Entrées: (%d, %d) Attendue: %i Sortie: %f\n", x[i][0], x[i][1], yy[i], final_output);
  
  }


  free(layer_inputs);
  free(layer_output);

  free(w);
  free(b);

  return 0;
}
