#include "utils/dstrings.h"

#include <stdlib.h>

#define UINT_SIZE (sizeof(unsigned int) * 8)

static const unsigned int p2[32] = {
    1,         2,         4,          8,         16,       32,       64,
    128,       256,       512,        1024,      2048,     4096,     8192,
    16384,     32768,     65536,      131072,    262144,   524288,   1048576,
    2097152,   4194304,   8388608,    16777216,  33554432, 67108864, 134217728,
    268435456, 536870912, 1073741824, 2147483648};

char *uint_to_binary(unsigned int n) {
  char tmp[UINT_SIZE + 1];
  unsigned int nb = n, pos = 0;

  do {
    tmp[pos++] = (n & 1) ? '1' : '0';
    n >>= 1;
  } while (p2[pos] <= nb);

  char *result = calloc(pos, sizeof(char));
  for (unsigned int i = 0; i < pos; i++)
    result[i] = tmp[pos - 1 - i];

  result[pos] = '\0';
  return result;
}
