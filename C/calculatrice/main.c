#include <stdio.h>
#include <stdlib.h>
#include <string.h>


double Request(char chaine[]) {

  char input[10];
  double number;

  printf(chaine);
  scanf("%lf", &number);

  return number;
}


int main() {
  
  double number1 = Request("Veuillez entrer un numéro: ");
  double number2 = Request("Veuillez entrer le 2ème numéro: ");
  
  double result = number2 + number1;
  
  printf("result: %lf", result);

  return 0;

}
