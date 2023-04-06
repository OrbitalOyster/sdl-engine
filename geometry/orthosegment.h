#ifndef ORTHOSEGMENT_H
#define ORTHOSEGMENT_H

#include "ortholine.h"
#include "point.h"

typedef struct {
  Point *p1;
  Point *p2;
  OrthoLine *line;
} OrthoSegment;

OrthoSegment *createHeapOrthoSegment(Point *p1, Point *p2);

#endif
