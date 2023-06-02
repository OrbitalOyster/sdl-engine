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

typedef enum {
  RMT_NONE,
  RMT_DIVERGE,
  RMT_CONVERGE,
  RMT_SLIP,
} RelativeMovementType;

typedef struct {
  OrtoRectCollisionType type;
  uint8_t edgeCollisionMask;
//  uint16_t orthoCollisionMask;
} OrthoRectCollision;

typedef struct {
  double time;
  uint8_t mask;
} OrthoRectCollisionChange;

RelativeMovementType getOrthoRectsRelativeMovementType(OrthoRect *r1,
                                                       OrthoRect *r2,
                                                       double vx1, double vy1,
                                                       double vx2, double vy2);
OrthoRectCollision getOrthoRectCollision(OrthoRect *r1, OrthoRect *r2);
uint8_t getMovingOrthoRectsImmediateCollisionChange(OrthoRect *r1,
                                                    OrthoRect *r2, double vx1,
                                                    double vy1, double vx2,
                                                    double vy2);
double getMovingOrthoRectsNextCollisionTime(OrthoRect *r1,
                                            OrthoRect *r2, double vx1,
                                            double vy1, double vx2,
                                            double vy2);
bool isCornerCollisionMask(uint8_t mask);
OrthoRectCollisionChange getMovingOrthoRectsNextCollisionChange(OrthoRect *r1,
                                                    OrthoRect *r2, double vx1,
                                                    double vy1, double vx2,
                                                    double vy2);
#endif
