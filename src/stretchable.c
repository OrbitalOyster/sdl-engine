#include "stretchable.h"

struct Stretchable *create_stretchable() {
  struct Stretchable *result = calloc(1, sizeof(struct Stretchable));
  return result;
}
