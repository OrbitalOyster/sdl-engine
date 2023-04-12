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
  uint8_t edgeCollisionMask;
  uint16_t orthoCollisionMask;
} OrthoRectCollision;

OrthoRectCollision getOrthoRectCollision(OrthoRect *r1, OrthoRect *r2);
uint8_t getMovingOrthoRectsImmediateCollisionChange(OrthoRect *r1,
                                                    OrthoRect *r2, double vx1,
                                                    double vy1, double vx2,
                                                    double vy2);
 
#endif
