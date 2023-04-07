#ifndef ORTHOLINE_H
#define ORTHOLINE_H

#include <stdbool.h>

#include "point.h"

typedef struct {
  bool isVertical;
  double xy;
} OrthoLine;

OrthoLine *createHeapOrthoLine(Point p1, Point p2);
void jumpOrthoLine(OrthoLine *line, double x, double y);
void moveOrthoLine(OrthoLine *line, double dx, double dy);

#endif
