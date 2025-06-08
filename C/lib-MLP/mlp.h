#ifndef MLP_H
#define MLP_H

#include <stdbool.h>

// Nombre de types d'activation supportés
#define NB_ACTIVATION_TYPES 3

// Type d'activation
typedef enum {
  ACTIVATION_SIGMOID,
  ACTIVATION_RELU,
  ACTIVATION_TANH
} ActivationType;

// Structure pour la configuration du MLP
typedef struct {
  int nb_layers;               // Nombre de couches
  int *nb_neurons;             // Nombre de neurones par couche
  ActivationType *activations; // Fonction d'activation par couche
  double learning_rate;        // Taux d'apprentissage
  int max_epochs;              // Nombre maximum d'époques
  bool verbose;                // Mode verbose
} MLPConfig;

// Structure pour représenter un MLP
typedef struct {
  MLPConfig config;            // Configuration du MLP
  int nb_layers;               // Nombre de couches
  int *nb_neurons;             // Nombre de neurones par couche
  int *inputs_per_layer;       // Nombre d'entrées par couche
  double ***weights;           // Poids [couche][neurone][entrée]
  double **biases;             // Biais [couche][neurone]
  ActivationType *activations; // Fonction d'activation par couche
  double (*activation_funcs[NB_ACTIVATION_TYPES])(double); // Pointeurs vers fonctions d'activation
  double (*activation_derivs[NB_ACTIVATION_TYPES])(double); // Pointeurs vers dérivées
} MLP;

// Fonction pour créer une configuration par défaut
MLPConfig *mlp_config_create(int nb_layers, const int *nb_neurons, const ActivationType *activations, double learning_rate, int max_epochs, bool verbose);

// Fonction pour libérer la configuration
void mlp_config_free(MLPConfig *config);

// Fonction pour créer un MLP à partir d'une configuration
MLP *mlp_create_with_config(const MLPConfig *config, int nb_inputs);

// Fonction pour libérer la mémoire du MLP
void mlp_free(MLP *mlp);

// Fonction pour effectuer une prédiction
double mlp_predict(MLP *mlp, const double *input, bool verbose);

// Fonction pour entraîner le MLP
void mlp_train(MLP *mlp, const double *inputs, const double *targets, int nb_samples, int nb_inputs);

// Fonction pour afficher les poids et biais
void mlp_print_weights(MLP *mlp);

#endif // MLP_H

// By Notpunchnox