// Reccurent neural network
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define learning_rate = 0.1
#define max_epochs 5000
#define nb_layers 2
#define nb_inputs 2

const int nb_neurons_per_layer[nb_layers] = {2, 1};

double Loss(double prediction, double sortie_attendue) {
    const double elipson = 1e-15;
    prediction = fmax(elipson, fmin(1.0 - elipson, prediction));
    return -(sortie_attendue * log(prediction) + (1.0 - sortie_attendue) * log(1.0 - prediction));
}

double sigmoide(double x) {
    return 1.0 / (1.0 + exp(x));
}

double sigmoide_derivative(double x) {
    double sig = sigmoide(x);
    return sig * (1.0 - sig);
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


int main() {
    srand(time(NULL));

    // Définir les poids et biais dans la mémoire afin d'avoir des tableaux dynamiques
    double **b = (double **)malloc(nb_layers * sizeof(double *));
    double ***w = (double ***)malloc(nb_layers * sizeof(double **));
    double **layer_inputs = (double **)malloc(nb_layers * sizeof(double *));
    double **layer_output = (double **)malloc(nb_layers * sizeof(double *));

    InitWeights(w, b);

    PrintNeuralNetwork(w, b);


    return 0;
}