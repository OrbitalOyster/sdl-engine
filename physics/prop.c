#include "prop.h"

#include <stdlib.h>

Prop *createProp(double x, double y, double w, double h) {
  Prop *prop = calloc(1, sizeof(Prop));
  *prop = (Prop) {.x = x, .y = y, .w = w, .h = h};
  prop->rect = createOrthoRect(x, y, w, h);
  return prop;
}

void freeProp(Prop *prop) {
  freeOrthoRect(prop->rect);
  free(prop);
}

