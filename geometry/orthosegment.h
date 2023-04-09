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
bool checkOrthoSegmentsIntersecting(OrthoSegment s1, OrthoSegment s2, bool excludeEndPoints);
bool pointBelongsToOrthoSegment(Point p, OrthoSegment s, bool excludeEndPoints);

#endif
