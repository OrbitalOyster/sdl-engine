#include "binary.h"

#include <stdlib.h>

char* int8ToBinary(int n) {
  char *result = calloc(9, sizeof(char));
  for (int i = 0; i < 8; i++, n >>= 1)
    result[7 - i] = n & 1 ? '1' : '0';
  result[8] = '\0';
  return result;
}

