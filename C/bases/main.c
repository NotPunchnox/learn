#include <stdio.h>
#include <string.h>

void foo() {
  char buffer[100];
  printf("Entrez un texte: ");

  fgets(buffer, sizeof(buffer), stdin);

  printf("%s\n", buffer);
}

int main() {
  foo();
  printf("Programme terminé avec succès !");
  return 0;
}
