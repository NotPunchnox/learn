#include <math.h>
#include <stdio.h>


// Fonction de calcul de la perte (log-vraisemblance négative)
// Formule : -[sortie_attendue * log(prediction) + (1 - sortie_attendue) * log(1
// - prediction)]
double Loss(double prediction, double sortie_attendue) {
  // Ajout de elipson constante pour éviter log(0)
  const double epsilon = 1e-15;
  prediction = fmax(epsilon, fmin(1.0 - epsilon, prediction));
  return -(sortie_attendue * log(prediction) +
           (1.0 - sortie_attendue) * log(1.0 - prediction));
}

// Fonction d'activation sigmoïde
double sigmoid(double y) { return 1.0 / (1.0 + exp(-y)); }

// Dérivée de la fonction sigmoïde
double sigmoid_derivative(double y) {
  double sig = sigmoid(y);
  return sig * (1.0 - sig);
}

int main() {

  const int nb_inputs = 2;
  const int nb_output = 1;
  const int nb_layers = 2;
  const int epochs = 500;
  const double learning_rate = 0.1;

  // Initialisation des données d'entrainement
  const int x[4][2] = {
    {0, 0}, 
    {1, 0}, 
    {0, 1}, 
    {1, 1}
  };
  const int yy[4] = {0, 1, 1, 0};

  // Nombre de neurons par couche
  int nb_neurons[] = {3, 3};


  // Initialisation des poids et biais
  double w[nb_layers][sizeof(nb_neurons) / sizeof(nb_neurons)[0]];
  double b[nb_layers][sizeof(nb_neurons) / sizeof(nb_neurons)[0]];

  // Initialisation aléatoire des poids et biais
  

  return 0;
}