#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define learning_rate 0.1
#define max_epochs 5000
#define nb_layers 2
#define nb_inputs 2

const int nb_neurons_per_layer[nb_layers] = {2, 1};

// Fonction de calcul de la perte (log-vraisemblance négative)
double Loss(double prediction, double sortie_attendue) {
  const double epsilon = 1e-15;
  prediction = fmax(epsilon, fmin(1.0 - epsilon, prediction));
  return -(sortie_attendue * log(prediction) + (1.0 - sortie_attendue) * log(1.0 - prediction));
}

// Fonction d'activation sigmoïde
double sigmoid(double y) {
  return 1.0 / (1.0 + exp(-y));
}

// Dérivée de la fonction sigmoïde
double sigmoid_derivative(double y) {
  double sig = sigmoid(y);
  return sig * (1.0 - sig);
}

int main() {

  // Jeu de données
  const int x[4][2] = {
      {0, 0},
      {0, 1},
      {1, 0},
      {1, 1},
  };
  const int yy[4] = {0, 1, 1, 0};

  // Définir les poids et biais dans la mémoire afin d'avoir des tableaux dynamiques
    double **b = (double **)malloc(nb_layers * sizeof(double));
    double ***w = (double ***)malloc(nb_layers * sizeof(double));

  // Lister les couches
  for (int layer = 0; layer < nb_layers; layer++) {

    // Lister les neurones présent dans la couche et leurs attitrer un biais
    for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron ++) {
      *b = (double *)malloc(nb_neurons_per_layer[layer] * sizeof(double));
      **w = *(double **)malloc(nb_neurons_per_layer[layer] * sizeof(double));

      b[layer][neuron] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;

      // Lister le nombre d'entrées
      for (int input = 0; input < (layer == 0 ? nb_inputs : nb_neurons_per_layer[layer -1]); input ++) {
        w[layer][neuron][input] = (double)rand() / RAND_MAX;
      }

    }
 
  };
  
  // Print weights (w) and biases (b)
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

  /*
  // Lister le jeu de données
  for (int i = 0; i < sizeof(x) / sizeof(x)[0]; i++) {

    // Lister les couches du réseau
    for (int layer = 0; layer < nb_layers; layer++) {

      double **layer_inputs = (double *)malloc(nb_neurons_per_layer[layer] *
  sizeof(double)); double **layer_output = (double
  *)malloc(nb_neurons_per_layer[layer] * sizeof(double));

      for (int neuron = 0; neuron < nb_neurons_per_layer[layer]; neuron++) {
        layer_inputs[layer][neuron] = b[layer][neuron]
      }

    }

  }*/

  return 0;
}