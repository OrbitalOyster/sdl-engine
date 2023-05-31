#ifndef PHYSICS_CALLBACK
#define PHYSICS_CALLBACK

#include <stdint.h>

#include "../geometry/orthorect.h"

typedef struct {
  OrthoRect *r1;
  OrthoRect *r2;
  double vx1;
  double vy1;
  double vx2;
  double vy2;
  double *avx;
  double *avy;
  uint8_t collisionChangeMask;
} physicsCallbackStats;

#endif
