// Importation des librairies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Déclaration de la fonction Request pour afficher un phrase et transtyper
double Request(char chaine[]) {

  // Déclaration des variables
  char input[10];
  double number;

  // Afficher la chaine de caractère mentionné en param
  printf(chaine);
  scanf("%lf", &number);

  // Retourner le numéro
  return number;
}

// Déclaration de la fonction de calcul
double Calc(double nb, double nb2) {

  // Déclaration des variables
  int input;
  double result;

  // Afficher le menu de calcul
  printf("Numéro 1: %lf\nNuméro 2: %lf\n\n", nb, nb2);
  printf("\nChoix opérateur:\nAddition (+): 1\nSoustraction (-): 2\nMultiplication (*): 3\nDivision (/): 4\n");
  printf("\nVous: ");
  scanf("%d", &input);


  // Choix de l'opérateur en fonction de l'entrée
  switch (input) {

    case 1:
      result = nb + nb2;
      break;

    case 2:
      result = nb - nb2;
      break;
    
    case 3:
      result = nb * nb2;
      break;

    case 4:
      result = nb / nb2;
      break;

    // Message d'erreur en cas de mauvaise entrée
    default:
      printf("\nErreur: la valeur %s n'est pas dans la plage {1, 2, 3, 4} !");
      result = 0;

  }

  // Retourner le résultat finale
  return result;

}

// Fonction main
int main() {
 
  printf("--- Calculatrice en C ---\n");

  // Appeler les fonctions Requests pour intérroger l'user, et calc pour effectuer le calcul
  double number1 = Request("Premier numéro: ");
  double number2 = Request("Second numéro: ");
  double result  = Calc(number1, number2);

  // Afficher le résultat
  printf("result: %lf", result);

  // Fermer le programme
  return 0;

}
