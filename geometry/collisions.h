#ifndef GEOMETRY_COLLISIONS
#define GEOMETRY_COLLISIONS

#include <stdbool.h>
#include <stdint.h>

#include "orthorect.h"

typedef enum {
  NO_COLLISION,
  OUTER_TOUCH,
  GHOST,
  INNER_TOUCH,
  INSIDE,
  OUTSIDE
} OrtoRectCollisionType;

typedef struct {
  OrtoRectCollisionType type;
  uint8_t borderCollisionMask;
  uint16_t orthoCollisionMask;
} OrthoRectCollision;

#endif
