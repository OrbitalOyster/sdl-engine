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
void jumpOrthoRect(OrthoRect *rect, double x, double y);
void moveOrthoRect(OrthoRect *rect, double dx, double dy);
void freeOrthoRect(OrthoRect *rect);

#endif
