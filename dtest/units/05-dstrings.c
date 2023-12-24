// obj/utils/dstrings.o
// 0

#include "utils/dstrings.h"

#include "utils/debug.h"

int main() {
  unsigned int n = 255;
  char *s = uint_to_binary(n);
  INFOF("Result: %s", s);
}
