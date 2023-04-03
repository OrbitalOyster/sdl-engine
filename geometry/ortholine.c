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

