#include <iostream>
#include <cmath>

using namespace std;

// Fonction de calcul du taux d'erreur (Log vraisemblance négative)
// y -> prediction ; y_ -> sortie attentue
double Loss(double y, double y_) {
  return (y_ * log(y) + (1 - y_) * log(1-y));
}

double sigmoide(double x) {
  return 1 / (1 + exp(-x));
}


int main() {

  // test de la fonction loss
  //cout << Loss(0.2, 0.1) << endl;

  // Dataset
  double x[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}
  };
  double yy[4] = { 0, 0, 0, 1};

  // poids
  double w[2] = {0.5, -1};
  double b = 1;
  double learning_rate = 0.01;


  // Boucle d'entrainement
  for (int epoch = 0; epoch < 50000; epoch++) {

    // lister les données ( boucle sur la dataset )
    for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++) {

      // Calcul de la sortie du perceptron
      double y = (x[i][0] * w[0]) + (x[i][1] * w[1]);
      double out = sigmoide(y);
      
      double e = Loss(out, yy[i]);

      if (epoch % 500 == 0) {
        cout << "Epoch: " << epoch << " | " << "Loos: " << e << endl;
      }

      // Mise à jour des poids et du biais
      for (int j = 0; j < sizeof(w) / sizeof(w[0]); j++) {

        // mise à jours des poids
        w[j] = w[j] - learning_rate * (out - yy[i]) * x[i][j];

        // mis à jours du biais
        b += b - (learning_rate * (out - yy[i]));

      }

    }

  }

  cout << "Fin de l'entrainement !" << endl;

  for (int i = 0; i < sizeof(x) / sizeof(x[0]); i++) {

    double y = (x[i][0] * w[0]) + (x[i][1] * w[1]);
    double out = sigmoide(y);

    cout << "[" << x[i][0] << " " << x[i][1] << "] -> " << yy[i] << " | sortie perceptron: " << out << endl;
  }

}
