#include <stdbool.h>
#include <stdio.h>

#include "../dmath/dmath.h"

bool compare_test() {
  double a = 0.1;
  double b = 0.2;
  bool result = compare(a + b, 0.3);
  printf("compare(a + b, 0.3) -> %i\n", result);
  return result;
}

int main() {
  compare_test();
}

