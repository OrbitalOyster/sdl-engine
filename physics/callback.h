#ifndef PHYSICS_CALLBACK
#define PHYSICS_CALLBACK

#include <stdint.h>

#include "../geometry/orthorect.h"

typedef struct {
  OrthoRect *r1;
  OrthoRect *r2;
  double *vx;
  double *vy;
  uint8_t collisionChangeMask;
} physicsCallbackStats;

#endif
