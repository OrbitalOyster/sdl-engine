#include "line.h"

#include <math.h>

#include "../debug.h"
#include "../dmath/dmath.h"

Line createLineFromPoints(Point p1, Point p2) {
  if (comparePoints(p1, p2))
    WARN("ILLEGAL OPERATION");
  Line result;
  // Vertical 
  if (compare(p1.x, p2.x))
    result = (Line){.y0 = NAN, .k = INFINITY, .x0 = p1.x};
  // Horizontal
  else if (compare(p1.y, p2.y))
    result = (Line){.y0 = p1.y, .k = 0, .x0 = NAN};
  // Inclined
  else {
    result.k = (p2.y - p1.y) / (p2.x - p1.x);
    result.y0 = p1.y - result.k * p1.x;
    result.x0 = -result.y0 / result.k;
  }
  return result;
}

double getLineX(Line l, double y) {
#ifdef GEOMETRY_DEBUG
  if (compare(l.k, 0))
    WARN("Getting X of horizontal line");
#endif
  return l.x0 + y / l.k;
}

double getLineY(Line l, double x) {
#ifdef GEOMETRY_DEBUG
  if (isinf(l.k))
    WARN("Getting Y of vertical line");
#endif
  return l.y0 + l.k * x;
}

