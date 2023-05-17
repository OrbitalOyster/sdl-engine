#ifndef PROP_H
#define PROP_H

#include <stdint.h>

#include "../geometry/orthorect.h"

typedef struct {
  // Dimensions
  double x;
  double y;
  double w;
  double h;
  OrthoRect* rect;
  // Collision parameters
  uint8_t collisionId;
  // Tag
  uint16_t tag;
} Prop;

Prop *createProp(double x, double y, double w, double h);
void freeProp(Prop* prop);

#endif
