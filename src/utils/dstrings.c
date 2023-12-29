#include "utils/dstrings.h"

#include <stdlib.h>

#define UINT_SIZE (sizeof(unsigned int) * 8)

static const unsigned int p2[32] = {
    1,          3,         7,         15,        31,        63,
    127,        255,       511,       1023,      2047,      4095,
    8191,       16383,     32767,     65535,     131071,    262143,
    524287,     1048575,   2097151,   4194303,   8388607,   16777215,
    33554431,   67108863,  134217727, 268435455, 536870911, 1073741823,
    2147483647, 4294967295};

char *uint_to_binary(unsigned int n) {
  char tmp[UINT_SIZE + 1];
  unsigned int nb = n, pos = 0;

  do {
    tmp[pos] = (n & 1) ? '1' : '0';
    n >>= 1;
  } while (p2[pos++] < nb);

  char *result = calloc(pos + 1, sizeof(char));
  for (unsigned int i = 0; i < pos; i++)
    result[i] = tmp[pos - 1 - i];

  result[pos] = '\0';
  return result;
}
