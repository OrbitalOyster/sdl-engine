#include "orthosegment.h"

#include <stdlib.h>

#include "../debug.h"
#include "../dmath/dmath.h"
#include "point.h"

OrthoSegment *createHeapOrthoSegment(Point *p1, Point *p2) {
#ifdef GEOMETRY_DEBUG
  // Special case
  if (comparePoints(*p1, *p2))
    WARN("Creating zero-length segment");

  if (!compareEps(p1->x, p2->x) && !compareEps(p1->y, p2->y))
    WARN("Attempting to create non-ortho segment");
#endif
  OrthoSegment *result = calloc(1, sizeof(OrthoSegment));
  result->p1 = p1;
  result->p2 = p2;
  // Make a line out of two points
  result->line = createHeapOrthoLine(*p1, *p2);
  return result;
}

