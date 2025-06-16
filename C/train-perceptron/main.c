/*
* Copyright (c)
* 2025 NotPunchnox
*/

// Importation de la lib math
#include <stdio.h>
#include <math.h>

// Fonciton de calcul du taux d'erreur ( Log vraisemblance négative )
// formule: -(sortie_attendue * log(prediction) + (1 - sortie_attendue) * log(1 - prediction))
double Loss(double prediction, double sortie_attendue) {
  double result = -(sortie_attendue * log(prediction)) + ((1 - sortie_attendue) * log(1 - prediction));
  return result;
}

// Fonction d'activation ( sigmoïde )
double activation(double y) {
  return 1 / (1 + exp(-y));
}

// Fonction principale
int main() {
  
  // Déclaration des variables

  // exemples de données sur l'opérateur logique AND
  double x[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}
  };
  double yy[4] = {
    0, // 0 AND 0 = 0
    0, // 0 AND 1 = 0
    0, // 1 AND 0 = 0
    1  // 1 AND 1 = 1
  };

  double w[2] = {0.5, -1};
  double b = 1;
  double learning_rate = 0.1;

  // Boucle d'entrainement
  for (int epoch = 0; epoch < 5000; epoch++) {

    // boucle sur les données
    for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++) {
      // Calcul de la sortie du perceptron
      double y = (x[i][0] * w[0]) + (x[i][1] * w[1]) + b;
      double p = activation(y);

      double error = Loss(p, yy[i]);
      
      // Mise à jour des poids et du biais
      for (int j = 0; j < sizeof(w) / sizeof(w)[0]; j++) {
        w[j] = w[j] - learning_rate * (p - yy[i]) * x[i][j];
      }
      
      // Mise à jour du biais
      b = b - learning_rate * (p - yy[i]);

    }

    // Affichage des poids et du biais
    printf("Epoch %d: w1 = %f, w2 = %f, b = %f\n", epoch, w[0], w[1], b);

  }

  // Affichage des poids finaux
  printf("Final weights: w1 = %f, w2 = %f, b = %f\n", w[0], w[1], b);

  // Test du perceptron sur les données d'entrainement
  for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++) {
    double y = (x[i][0] * w[0]) + (x[i][1] * w[1]) + b;
    double p = activation(y);

    printf("Input: (%f, %f) => Prediction: %f, Expected: %f\n", x[i][0], x[i][1], p, yy[i]);
  }

  return 0;
}