#include "binary.h"

#include <stdlib.h>

char* intToBinary(int n, unsigned int size) {
  char *result = calloc(size + 1, sizeof(char));
  for (unsigned int i = 0; i < size; i++, n >>= 1)
    result[size - 1 - i] = n & 1 ? '1' : '0';
  result[size] = '\0';
  return result;
}

