#include "mlp.h"
#include <stdio.h>

int main() {
  // Données d'entraînement pour XOR
  double inputs[] = {0, 0, 0, 1, 1, 0, 1, 1};
  double targets[] = {0, 1, 1, 0};
  int nb_samples = 4;
  int nb_inputs = 2;

  // Configuration du MLP
  int nb_layers = 2;
  int nb_neurons[] = {3, 1}; // 3 neurones cachés, 1 neurone de sortie
  ActivationType activations[] = {ACTIVATION_RELU, ACTIVATION_SIGMOID};
  MLPConfig *config = mlp_config_create(nb_layers, nb_neurons, activations, 0.3, 5000, true);

  if (!config) {
    fprintf(stderr, "Erreur: Échec de la création de la configuration\n");
    return 1;
  }

  // Création du MLP
  MLP *mlp = mlp_create_with_config(config, nb_inputs);
  if (!mlp) {
    fprintf(stderr, "Erreur: Échec de la création du MLP\n");
    mlp_config_free(config);
    return 1;
  }

  // Entraînement
  mlp_train(mlp, inputs, targets, nb_samples, nb_inputs);


  // Test final
  printf("\nTest sur les données d'entraînement :\n");
  for (int i = 0; i < nb_samples; i++) {
    double input[2] = {inputs[i * 2], inputs[i * 2 + 1]};
    double prediction = mlp_predict(mlp, input, false);
    printf("Entrée: (%f, %f) => Prédiction: %f, Attendu: %f\n", input[0], input[1], prediction, targets[i]);
  }

  // Libération de la mémoire
  mlp_free(mlp);
  mlp_config_free(config);
  return 0;
}
