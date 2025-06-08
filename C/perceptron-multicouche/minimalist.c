#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Fonction de calcul du taux d'erreur (log-vraisemblance négative )
// Formule: -[sortie attendue * log(prediction) + (1 - sortie attendue) * log(1
// - prediction)]
double Loss(double prediction, double sortie_attentue) {
  const double elipson = 1e-15;
  prediction = fmax(elipson, fmin(1.0 - elipson, prediction));
  return (sortie_attentue * log(prediction) +
          (1.0 - sortie_attentue) * log(1.0 - prediction));
}

// Fonction d'activation sigmoïde
double sigmoide(double y) { return 1.0 / (1.0 + exp(-y)); }

// Dérivée de la fonction sigmoïde
double sigmoide_derivative(double y) {
  double sig = sigmoide(y);
  return sig * (1.0 - sig);
}

// Fonction principale
int main() {

  const double learning_rate = 0.1;
  const int max_epochs = 50000;
  const int nb_neurons_layer1 = 3;
  const int nb_neurons_layer2 = 2;
  const int nb_inputs = 2;

  // Données d'entraînement pour XOR
  double x[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
  double yy[4] = {0, 1, 1, 0};

  // Initialisation des poids et biais
  double w[2][2][3]; // 2 couches, 2 neurones dans la couche 2, 3 entrées dans
                     // la couche 1
  double b[2][2];    // 2 couches, 2 neurones dans la couche 2

  // Initialisation aléatoire des poids et biais
  for (int layer = 0; layer < 2; layer++) {
    for (int neuron = 0;
         neuron < (layer == 0 ? nb_neurons_layer1 : nb_neurons_layer2);
         neuron++) {
      for (int input = 0; input < (layer == 0 ? nb_inputs : nb_neurons_layer1);
           input++) {
        // Poids aléatoires entre [-0.1, 0.1]
        w[layer][neuron][input] = ((double)rand() / RAND_MAX) * 0.2 - 0.1;
      }
      // Biais initialisé à 1
      b[layer][neuron] = 1;
    }
  }

  // Boucle d'entraînement
  for (int epoch = 0; epoch < max_epochs; epoch++) {
    double total_error = 0.0;

    for (int i = 0; i < 4; i++) {
      // Couche 1
      double *layer_inputs = malloc(nb_neurons_layer1 * sizeof(double));
      double *layer_outputs = malloc(nb_neurons_layer1 * sizeof(double));

      // Calcul des entrées et sorties de la couche 1
      // j = neurones de la couche 1
      // k = entrées de la couche 1
      for (int j = 0; j < nb_neurons_layer1; j++) {
        layer_inputs[j] = b[0][j];
        for (int k = 0; k < nb_inputs; k++) {
          // Ajout de la somme des entrées & poids au biais
          layer_inputs[j] += x[i][k] * w[0][j][k];
        }

        // passage des valeurs dans la fonction d'activation
        layer_outputs[j] = sigmoide(layer_inputs[j]);
      }

      // Calcul de la sortie de la couche 2
      // j = neurones de la couche 2
      double final_output = b[1][0];
      for (int j = 0; j < nb_neurons_layer1; j++) {
        // Ajout de la somme des sorties de la couche 1 & poids au biais
        final_output += layer_outputs[j] * w[1][0][j];
      }
      // passage des valeurs dans la fonction d'activation
      final_output = sigmoide(final_output);

      // Calcul de la perte
      total_error += Loss(final_output, yy[i]);

      // Rétropropagation
      double delta = (final_output - yy[i]) * sigmoide_derivative(final_output);

      // delta est la dérivée de la fonction de perte par rapport à la sortie
      // j = neurones de la couche 2
      for (int j = 0; j < nb_neurons_layer1; j++) {
        w[1][0][j] -= learning_rate * delta * layer_outputs[j];
      }
      // Mise à jour du biais de la couche 2
      b[1][0] -= learning_rate * delta;

      // Rétropropagation vers la couche 1
      double *delta_hidden = malloc(nb_neurons_layer1 * sizeof(double));

      // delta_hidden est la dérivée de la fonction de perte par rapport aux
      // entrées de la couche 1
      // j = neurones de la couche 1
      // k = entrées de la couche 1
      for (int j = 0; j < nb_neurons_layer1; j++) {
        delta_hidden[j] =
            delta * w[1][0][j] * sigmoide_derivative(layer_inputs[j]);
        for (int k = 0; k < nb_inputs; k++) {
          w[0][j][k] -= learning_rate * delta_hidden[j] * x[i][k];
        }
        b[0][j] -= learning_rate * delta_hidden[j];
      }

      // Libération de la mémoire
      free(delta_hidden);
      free(layer_outputs);
      free(layer_inputs);
    }

    // Affichage toutes les 1000 époques
    if (epoch % 1000 == 0) {
      printf("Epoch %d, Total Error: %f\n", epoch, total_error);
    }
  }

  // Test du modèle
  for (int i = 0; i < 4; i++) {
    double layer_inputs[nb_neurons_layer1];
    double layer_outputs[nb_neurons_layer1];

    // Couche 1
    for (int j = 0; j < nb_neurons_layer1; j++) {
      layer_inputs[j] = b[0][j];
      for (int k = 0; k < nb_inputs; k++) {
        layer_inputs[j] += x[i][k] * w[0][j][k];
      }
      layer_outputs[j] = sigmoide(layer_inputs[j]);
    }

    // Couche 2
    double final_output = b[1][0];
    for (int j = 0; j < nb_neurons_layer1; j++) {
      final_output += layer_outputs[j] * w[1][0][j];
    }
    final_output = sigmoide(final_output);

    printf("Input: (%f, %f), Predicted Output: %f, Expected Output: %f\n",
           x[i][0], x[i][1], final_output, yy[i]);
  }

  return 0;
}

// By NotPunchnox
