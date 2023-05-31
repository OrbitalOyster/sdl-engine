#ifndef ORTHOSEGMENT_H
#define ORTHOSEGMENT_H

#include <stdbool.h>

#include "ortholine.h"
#include "point.h"

typedef struct {
  Point *p1;
  Point *p2;
  OrthoLine *line;
} OrthoSegment;

OrthoSegment *createHeapOrthoSegment(Point *p1, Point *p2);
bool compareOrthoSegments(OrthoSegment s1, OrthoSegment s2);
bool checkOrthoSegmentsInterlacing(OrthoSegment s1, OrthoSegment s2);
bool checkOrthoSegmentsIntersecting(OrthoSegment s1, OrthoSegment s2);
bool pointBelongsToOrthoSegment(Point p, OrthoSegment s);
void freeOrthoSegment(OrthoSegment* s);

#endif
