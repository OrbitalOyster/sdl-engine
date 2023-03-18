#include "ortholine.h"

#include <stdlib.h>

#include "../dmath/dmath.h"

OrthoLine *createHeapOrthoLine(Point p1, Point p2) {
#ifdef GEOMETRY_DEBUG
  if (comparePoints(p1, p2))
    printf("GEOMENTRY_DEBUG: Attempting to create OrthoLine from two equal "
           "points\n");

  if (!compare(p1.x, p2.x) && !compare(p1.y, p2.y))
    printf("GEOMETRY_DEBUG: Attempting to create non-ortho line\n");
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
