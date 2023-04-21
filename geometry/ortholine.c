#include "ortholine.h"

#include <stdlib.h>

#include "../debug.h"
#include "../dmath/dmath.h"

OrthoLine *createHeapOrthoLine(Point p1, Point p2) {
#ifdef GEOMETRY_DEBUG
  if (comparePoints(p1, p2))
    WARN("Attempting to create OrthoLine from two equal points");

  if (!compare(p1.x, p2.x) && !compare(p1.y, p2.y))
    WARN("Attempting to create non-ortho line");
#endif
  OrthoLine *result = calloc(1, sizeof(OrthoLine));
  // Vertical line
  if (compare(p1.x, p2.x)) {
    result->isVertical = true;
    result->xy = p1.x;
  }
  // Horizontal line
  else {
    result->isVertical = false;
    result->xy = p1.y;
  }
  return result;
}

bool compareOrthoLines(OrthoLine l1, OrthoLine l2) {
  return l1.isVertical == l2.isVertical && compare(l1.xy, l2.xy);
}

bool compareLineOrthoLine(Line l1, OrthoLine l2) {
  return l2.isVertical ? compare(l2.xy, l1.x0) : compare(l2.xy, l1.y0);
}

void jumpOrthoLine(OrthoLine *line, double x, double y) {
  line->xy = line->isVertical ? x : y;
}

void moveOrthoLine(OrthoLine *line, double dx, double dy) {
  line->xy += line->isVertical ? dx : dy;
}

bool pointBelongsToOrthoLine(Point p, OrthoLine l) {
  return l.isVertical ? compare(p.x, l.xy) : compare(p.y, l.xy);
}

Point getOrthoLinesIntersection(OrthoLine l1, OrthoLine l2) {
#ifdef GEOMETRY_DEBUG
  if (l1.isVertical == l2.isVertical)
    WARN("No intersection of parallel lines");
#endif
  if (l1.isVertical)
    return (Point){.x = l1.xy, .y = l2.xy};
  else
    return (Point){.x = l2.xy, .y = l1.xy};
}

Point getOrthoLineLineIntersection(OrthoLine l1, Line l2) {
#ifdef GEOMETRY_DEBUG
  if ((l1.isVertical && l2.k == INFINITY) || (!l1.isVertical && l2.k == 0))
    WARN("Parallel lines");
#endif
  if (l1.isVertical)
    return (Point){.x = l1.xy, .y = getLineY(l2, l1.xy)};
  else
    return (Point){.x = getLineX(l2, l1.xy), .y = l1.xy};
}

