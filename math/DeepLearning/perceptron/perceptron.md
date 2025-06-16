# Le perceptron

## Définition
**Le perceptron**, ou **neurone formel**, est un **neurone artificiel**. Il sert à faire de l'apprentissage supervisé. Inventé en 1957 par Frank Rosenblatt, il peut résoudre des problèmes linéairement séparable, comme l'opération AND.

## Comment ça marche
Un perceptron prend des entrées $( x_1, x_2, \dots, x_n $), leur associe des poids $( w_1, w_2, \dots, w_n $), et calcule une sortie. Voici les étapes :
1. **Somme pondérée** : On calcule ( $z = \sum_{i=1}^{n} w_i x_i + b$ ), où $( b $) est le biais.
2. **Fonction d'activation** : On applique une fonction, souvent la fonction en échelon :
   $f(z) = \begin{cases} 
   1 & \text{si } z \geq 0 \\
   0 & \text{sinon}
   \end{cases}$
   La sortie est $( y = f(z) $).

## Illustration
![Schéma du perceptron](./perceptron.png)

## Apprentissage
Pour apprendre, le perceptron ajuste les poids et le biais :
1. On commence avec des poids aléatoires.
2. Pour chaque exemple $(x, y_{\text{vrai}}) $, on prédit $y_{\text{prédit}}$.
3. Si la prédiction est fausse, on met à jour :
   $w_i \gets w_i + \eta (y_{\text{vrai}} - y_{\text{prédit}}) x_i$

   $b \gets b + \eta (y_{\text{vrai}} - y_{\text{prédit}})$

   où $(\eta$) est le taux d'apprentissage (ex. : 0.1).


## Exemple : AND
Pour l'opération AND avec deux entrées $( x_1, x_2 \in \{0, 1\} $):

| $( x_1 $) | $( x_2 $) | Sortie (AND) |
|-----------|-----------|--------------|
| 0         | 0         | 0            |
| 0         | 1         | 0            |
| 1         | 0         | 0            |
| 1         | 1         | 1            |

Le perceptron trouve des poids (ex. : $( w_1 = 0.5, w_2 = 0.5, b = -0.7 $)) pour résoudre ce problème.

## Limites
Le perceptron ne peut résoudre que des problèmes linéaires. Par exemple, il échoue sur XOR :

| $( x_1 $) | $( x_2 $) | Sortie (XOR) |
|-----------|-----------|--------------|
| 0         | 0         | 0            |
| 0         | 1         | 1            |
| 1         | 0         | 1            |
| 1         | 1         | 0            |

C'est parce que XOR n'est pas linéairement séparable.

## Lien avec Deep Learning
Le perceptron est la base des réseaux de neurones. En combinant plusieurs perceptrons, on peut résoudre des problèmes complexes, comme en Deep Learning ou en robotique.

## Cours suivant (Perceptron multicouches)
[clique ici](./perceptron-multicouches.md)