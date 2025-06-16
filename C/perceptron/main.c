// Importation de la lib math
#include <stdio.h>
#include <math.h>

// Fonciton de calcul du taux d'erreur ( Log vraisemblance négative )
// formule: (sortie_attendue * log(prediction) + (1 - sortie_attendue) * log(1 - prediction))
double Loss(double prediction, double sortie_attendue) {
  double result = (sortie_attendue * log(prediction)) + ((1 - sortie_attendue) * log(1 - prediction));
  return result;
}

// Fonction d'activation ( sigmoïde )
double activation(double y) {
  return 1 / (1 + exp(-y));
}

// Fonction principale
int main() {

  // Déclaration des variables
  double x1, x2;
  double w1, w2;
  double b, y;

  double learning_rate = 0.1;
  double sortie_attendue = 1;

  // Initialisation des poids et du biais
  x1 = 1;
  x2 = 0;

  w1 = 0.5;
  w2 = -1;

  b = 1;

  // Calcul de la sortie du perceptron
  y = (x1 * w1) + (x2 * w2) + b;

  // Application de la fonction d'activation
  double prediction = activation(y);
  printf("Prediction: %f\n", prediction);

  // Calcul du taux d'erreur
  double error = Loss(prediction, sortie_attendue);
  printf("Taux d'erreur: %f\n", error);

  // Mise à jours des paramètres
  // formule:
  // i = index
  // wi <- wi - n * (prediction - sortie_attendue) * xi
  // b <- b - n * (prediction - sortie_attendue)
  w1 = w1 - learning_rate * (prediction - sortie_attendue) * x1;
  w2 = w2 - learning_rate * (prediction - sortie_attendue) * x2;
  b = b - learning_rate * (prediction - sortie_attendue);

  printf("Nouveaux poids: w1 = %f, w2 = %f, b = %f\n", w1, w2, b);

  return 0;

}


// by NotPunchnox