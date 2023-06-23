#ifndef PHYSICS_CALLBACK
#define PHYSICS_CALLBACK

#include <stdint.h>

#include "../geometry/orthorect.h"

typedef struct PhysicsCallback PhysicsCallback;

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
  // TODO: Better way to do this?
  PhysicsCallback *callback;
} physicsCallbackStats;

struct PhysicsCallback {
  uint8_t priority;
  void (*func)(physicsCallbackStats);
};

#endif
