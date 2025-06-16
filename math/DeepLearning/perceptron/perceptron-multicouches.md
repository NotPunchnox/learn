# Le Perceptron Multicouche (MLP)

## Définition
Le **perceptron multicouche** (ou **MLP**, Multi-Layer Perceptron) est un réseau de neurones artificiels qui étend le perceptron simple. Il est utilisé pour l'apprentissage supervisé et peut résoudre des problèmes **non linéairement séparables**, comme l'opération XOR. Introduit après les limites du perceptron simple, il est une base essentielle du Deep Learning.

## Comment ça marche
Un MLP est composé de plusieurs couches de neurones :
- **Couche d'entrée** : Reçoit les données $( x_1, x_2, \dots, x_n $).
- **Couches cachées** : Contiennent des neurones qui traitent les données avec des poids, biais et fonctions d'activation.
- **Couche de sortie** : Produit le résultat final (ex. : classification ou régression).

Chaque neurone fonctionne comme un perceptron simple :
1. **Somme pondérée** : Pour un neurone, on calcule $( z = \sum_{i=1}^{n} w_i x_i + b $), où $( w_i $) sont les poids et $( b $) le biais.
2. **Fonction d'activation** : On applique une fonction, souvent non linéaire comme la **sigmoïde** :
   $
   f(z) = \frac{1}{1 + e^{-z}}
   $
   ou la **ReLU** :
   $
   f(z) = \max(0, z)
   $
   La sortie du neurone est $( y = f(z) $).

Les couches cachées avec des fonctions non linéaires permettent de modéliser des relations complexes.

## Illustration
![Schéma du perceptron multicouche](./images/mlp_diagram.png)

## Apprentissage
Le MLP apprend en ajustant les poids et biais via la **rétropropagation de l'erreur** :
1. **Initialisation** : Les poids et biais sont initialisés aléatoirement.
2. **Propagation avant** : Pour chaque exemple $( (x, y_{\text{vrai}}) $), on calcule la sortie $( y_{\text{prédit}} $) en passant par toutes les couches.
3. **Calcul de l'erreur** : On mesure l'erreur avec une fonction de perte, comme l'erreur quadratique :
   $
   L = \frac{1}{2} (y_{\text{vrai}} - y_{\text{prédit}})^2
   $
4. **Rétropropagation** : On calcule les gradients de l'erreur par rapport aux poids et biais, puis on met à jour :
   $
   w_i \gets w_i - \eta \frac{\partial L}{\partial w_i}
   $
   $
   b \gets b - \eta \frac{\partial L}{\partial b}
   $
   où $( \eta $) est le taux d'apprentissage (ex. : 0.01).
5. **Répétition** : On répète jusqu'à convergence ou pour un nombre fixé d'itérations.

## Exemple : XOR
Contrairement au perceptron simple, le MLP peut résoudre l'opération XOR :

| $( x_1 $) | $( x_2 $) | Sortie (XOR) |
|-----------|-----------|--------------|
| 0         | 0         | 0            |
| 0         | 1         | 1            |
| 1         | 0         | 1            |
| 1         | 1         | 0            |

Un MLP avec une couche cachée (ex. : 2 neurones, fonction sigmoïde) peut apprendre une frontière non linéaire pour séparer les classes.

## Limites
- **Complexité** : Le MLP nécessite plus de calculs et de données qu'un perceptron simple.
- **Surapprentissage** : Sans régularisation, il peut mémoriser les données d'entraînement.
- **Temps d'entraînement** : La rétropropagation peut être lente pour des réseaux profonds.

## Lien avec Deep Learning
Le MLP est un réseau de neurones de base. En ajoutant plus de couches cachées, on obtient des réseaux profonds, utilisés en Deep Learning pour des tâches comme la reconnaissance d'images...

## Cours suivant (Réseaux de neurones profonds)
[clique ici](../deep-neural-network/reseaux-neurones-profonds.md)