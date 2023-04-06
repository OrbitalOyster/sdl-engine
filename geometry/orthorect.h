#ifndef ORTHORECT_H
#define ORTHORECT_H

#include "orthosegment.h"
#include "point.h"

typedef struct {
  double x;
  double y;
  double w;
  double h;
  Point *vertices[4];
  OrthoSegment *edges[4];
} OrthoRect;

OrthoRect *createOrthoRect(double x, double y, double w, double h);
void freeOrthoRect(OrthoRect *rect);

#endif
