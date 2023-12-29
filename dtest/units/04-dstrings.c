// obj/utils/dstrings.o
// 0

#include "utils/dstrings.h"

#include "utils/debug.h"

int main() {
  unsigned int n = 3;
  char *s = uint_to_binary(n);
  INFOF("%u: %s", n, s);
  free(s);
  n = 16;
  s = uint_to_binary(n);
  INFOF("%u: %s", n, s);
  free(s);
  n = 255;
  s = uint_to_binary(n);
  INFOF("%u: %s", n, s);
  free(s);
}
