#include "point.h"

#include "../dmath/dmath.h"

#include <stdlib.h>

Point *createHeapPoint(double x, double y) {
  Point *result = calloc(1, sizeof(Point));
  result->x = x;
  result->y = y;
  return result;
}

bool comparePoints(Point p1, Point p2) {
  return compare(p1.x, p2.x) && compare(p1.y, p2.y);
}

