#include <stdio.h>

#define RED "\033[33m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define BACK "\n"

int main() {

  printf(RED "Text rouge !!!" RESET BACK);
  printf(GREEN "Text vert !" RESET BACK);

  return 0;
}
