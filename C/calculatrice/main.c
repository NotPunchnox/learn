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
  
  double test = Request("Veuillez entrer un numéro: ");
  
  printf("result: %lf", test);

  return 0;

}
