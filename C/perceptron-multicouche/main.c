#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Fonction de calcul de la perte (log-vraisemblance négative)
// Formule : -[sortie_attendue * log(prediction) + (1 - sortie_attendue) * log(1 - prediction)]
double Loss(double prediction, double sortie_attendue) {
  // Ajout de elipson constante pour éviter log(0)
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

// Fonction principale
int main() {
  // Paramètres d'entraînement
  const double learning_rate = 0.1;
  const int max_epochs = 50000;
  const int nb_layers = 2;
  const int nb_neurons[] = {3, 2};
  const int nb_inputs = 2;

  // Données d'entraînement pour XOR
  double x[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  double yy[4] = {0, 1, 1, 0};

  // Initialisation des poids et biais>
  double ***w = malloc(nb_layers * sizeof(double **));
  double **b = malloc(nb_layers * sizeof(double *));
  int inputs_per_layer[] = {nb_inputs, nb_neurons[0]};

  for (int layer = 0; layer < nb_layers; layer++) {
    w[layer] = malloc(nb_neurons[layer] * sizeof(double *));
    b[layer] = malloc(nb_neurons[layer] * sizeof(double));
    for (int neuron = 0; neuron < nb_neurons[layer]; neuron++) {
      w[layer][neuron] = malloc(inputs_per_layer[layer] * sizeof(double));
      for (int input = 0; input < inputs_per_layer[layer]; input++) {
        // Poids aléatoires entre [-0.1, 0.1]
        w[layer][neuron][input] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
      }
      // Biais initialisé à 0.1
      b[layer][neuron] = 0.1;
    }
  }

  // Boucle d'entraînement
  for (int epoch = 0; epoch < max_epochs; epoch++) {
    double total_error = 0.0;

    for (int i = 0; i < 4; i++) {

      // Propagation avant
      double *layer_outputs = malloc(nb_neurons[0] * sizeof(double));
      double *layer_inputs = malloc(nb_neurons[0] * sizeof(double));

      // Couche 1
      for (int j = 0; j < nb_neurons[0]; j++) {
        layer_inputs[j] = b[0][j];
        for (int k = 0; k < nb_inputs; k++) {
          layer_inputs[j] += x[i][k] * w[0][j][k];
        }
        layer_outputs[j] = sigmoide(layer_inputs[j]);
      }

      // Couche 2
      double final_output = 0.0;
      double final_input = b[1][0];
      for (int j = 0; j < nb_neurons[0]; j++) {
        final_input += layer_outputs[j] * w[1][0][j];
      }
      final_output = sigmoide(final_input);

      // Calcul de la perte
      total_error += Loss(final_output, yy[i]);

      // Rétropropagation
      double delta = (final_output - yy[i]) * sigmoid_derivative(final_input);
      for (int j = 0; j < nb_neurons[0]; j++) {
        w[1][0][j] -= learning_rate * delta * layer_outputs[j];
      }
      b[1][0] -= learning_rate * delta;

      // Rétropropagation vers la couche 1
      double *delta_hidden = malloc(nb_neurons[0] * sizeof(double));
      for (int j = 0; j < nb_neurons[0]; j++) {
        delta_hidden[j] = delta * w[1][0][j] * sigmoid_derivative(layer_inputs[j]);
        for (int k = 0; k < nb_inputs; k++) {
          w[0][j][k] -= learning_rate * delta_hidden[j] * x[i][k];
        }
        b[0][j] -= learning_rate * delta_hidden[j];
      }

      free(delta_hidden);
      free(layer_outputs);
      free(layer_inputs);
    }

    // Affichage toutes les 1000 époques
    if (epoch % 1000 == 0) {
      printf("Époque %d, Erreur moyenne: %f\n", epoch, total_error / 4.0);
    }
  }

  // Affichage des poids finaux
  printf("\nPoids finaux :\n");
  for (int layer = 0; layer < nb_layers; layer++) {
    printf("Couche %d :\n", layer);
    for (int neuron = 0; neuron < nb_neurons[layer]; neuron++) {
      printf("  Neurone %d : ", neuron);
      for (int input = 0; input < inputs_per_layer[layer]; input++) {
        printf("w[%d][%d][%d] = %f ", layer, neuron, input, w[layer][neuron][input]);
      }
      printf("b[%d][%d] = %f\n", layer, neuron, b[layer][neuron]);
    }
  }

  // Test final
  printf("\nTest sur les données d'entraînement :\n");
  for (int i = 0; i < 4; i++) {
    double *layer_outputs = malloc(nb_neurons[0] * sizeof(double));
    double *layer_inputs = malloc(nb_neurons[0] * sizeof(double));
    for (int j = 0; j < nb_neurons[0]; j++) {
      layer_inputs[j] = b[0][j];
      for (int k = 0; k < nb_inputs; k++) layer_inputs[j] += x[i][k] * w[0][j][k];

      layer_outputs[j] = sigmoide(layer_inputs[j]);
    }
    double final_output = b[1][0];
    for (int j = 0; j < nb_neurons[0]; j++) {
      final_output += layer_outputs[j] * w[1][0][j];
    }
    final_output = sigmoide(final_output);
    printf("Entrée: (%f, %f) => Prédiction: %f, Attendu: %f\n", x[i][0], x[i][1], final_output, yy[i]);
    free(layer_outputs);
    free(layer_inputs);
  }

  // Libération de la mémoire
  for (int layer = 0; layer < nb_layers; layer++) {
    for (int neuron = 0; neuron < nb_neurons[layer]; neuron++) {
      free(w[layer][neuron]);
    }
    free(w[layer]);
    free(b[layer]);
  }
  free(w);
  free(b);

  return 0;
}
