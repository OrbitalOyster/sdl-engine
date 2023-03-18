#ifndef ORTHOSEGMENT_H
#define ORTHOSEGMENT_H

#include "ortholine.h"
#include "point.h"

typedef struct OrthoSegment {
  Point *p1;
  Point *p2;
  OrthoLine *line;
} OrthoSegment;

#endif

