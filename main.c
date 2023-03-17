#include <stdio.h>

#include "dmath/dmath.h"

int main() {
  printf("Starting engine...\n");
  double a = 0.1;
  double b = 0.2;
  double c = a + b;
  printf("c == 0.3 -> %i\n", c == 0.3);
  printf("compare(c, 0.3) -> %i\n", compare(c, 0.3));
  return 0;
}

