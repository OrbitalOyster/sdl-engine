#ifndef ORTHOLINE_H
#define ORTHOLINE_H

#include <stdbool.h>

#include "point.h"
#include "line.h"

typedef struct {
  bool isVertical;
  double xy;
} OrthoLine;

OrthoLine *createHeapOrthoLine(Point p1, Point p2);
bool compareOrthoLines(OrthoLine l1, OrthoLine l2);
bool compareLineOrthoLine(Line l1, OrthoLine l2);
void jumpOrthoLine(OrthoLine *line, double x, double y);
void moveOrthoLine(OrthoLine *line, double dx, double dy);
bool pointBelongsToOrthoLine(Point p, OrthoLine l);
Point getOrthoLinesIntersection(OrthoLine l1, OrthoLine l2);
Point getOrthoLineLineIntersection(OrthoLine l1, Line l2);

#endif
