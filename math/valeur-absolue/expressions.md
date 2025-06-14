Explications des expressions avec la valeur absolue

La **valeur absolue** d’un nombre \( x \), notée \( |x| \), représente la distance de \( x \) à zéro sur la droite réelle, sans tenir compte de son signe. Par exemple :
- \( |3| = 3 \),
- \( |-3| = 3 \),
- \( |0| = 0 \).

Ci-dessous, nous analysons deux expressions courantes impliquant la valeur absolue : \( |a + b| = |a| + |b| \) et \( ||a| - |b|| \).

### 1. Expression : \( |a + b| = |a| + |b| \)

Cette égalité **n’est pas toujours vraie**. Elle ne tient que dans des cas particuliers.

#### Signification
- \( |a + b| \) : Valeur absolue de la somme de \( a \) et \( b \), c’est-à-dire la distance de \( a + b \) à zéro.
- \( |a| + |b| \) : Somme des valeurs absolues de \( a \) et \( b \), c’est-à-dire la somme des distances de \( a \) et \( b \) à zéro.

#### Condition pour l’égalité
L’égalité \( |a + b| = |a| + |b| \) est vérifiée si :
- \( a \) et \( b \) ont le **même signe** (ou si l’un des deux est zéro).
- Exemples :
  - Si \( a = 2 \), \( b = 3 \), alors \( |2 + 3| = |5| = 5 \), et \( |2| + |3| = 2 + 3 = 5 \). L’égalité est vraie.
  - Si \( a = -2 \), \( b = -3 \), alors \( |-2 + (-3)| = |-5| = 5 \), et \( |-2| + |-3| = 2 + 3 = 5 \). L’égalité est vraie.
- L’égalité est **fausse** si \( a \) et \( b \) ont des signes opposés :
  - Si \( a = 2 \), \( b = -2 \), alors \( |2 + (-2)| = |0| = 0 \), mais \( |2| + |-2| = 2 + 2 = 4 \). Donc \( 0 \neq 4 \).

#### Propriété générale
L’égalité \( |a + b| = |a| + |b| \) est un cas particulier de l’**inégalité triangulaire** :
\[ |a + b| \leq |a| + |b| \]
L’égalité ne tient que lorsque \( a \) et \( b \) sont dans la même direction (même signe ou l’un est zéro).

### 2. Expression : \( ||a| - |b|| \)

Cette expression représente la **valeur absolue de la différence des valeurs absolues** de \( a \) et \( b \).

#### Signification
- On calcule d’abord \( |a| \) et \( |b| \), qui sont les distances de \( a \) et \( b \) à zéro.
- On prend la différence \( |a| - |b| \).
- Ensuite, on applique la valeur absolue à cette différence, ce qui donne \( ||a| - |b|| \).
- Résultat : \( ||a| - |b|| \) est la **distance entre les valeurs absolues** de \( a \) et \( b \), toujours positive ou nulle.

#### Exemples
- Si \( a = 5 \), \( b = 3 \), alors \( |a| = 5 \), \( |b| = 3 \), et \( ||a| - |b|| = |5 - 3| = |2| = 2 \).
- Si \( a = -5 \), \( b = 3 \), alors \( |a| = 5 \), \( |b| = 3 \), et \( ||a| - |b|| = |5 - 3| = |2| = 2 \).
- Si \( a = 3 \), \( b = 5 \), alors \( |a| = 3 \), \( |b| = 5 \), et \( ||a| - |b|| = |3 - 5| = |-2| = 2 \).

#### Propriété utile
Une inégalité importante relie cette expression à la différence \( a - b \) :
\[ ||a| - |b|| \leq |a - b| \]
Cela signifie que la distance entre les valeurs absolues de \( a \) et \( b \) est toujours inférieure ou égale à la valeur absolue de leur différence.

### Conclusion
Les expressions de valeur absolue comme ( |a + b| = |a| + |b| ) et ( ||a| - |b|| ) ont des significations géométriques (distances) et des conditions spécifiques pour leur validité.