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

bool checkOrthoSegmentsInterlacing(OrthoSegment s1, OrthoSegment s2) {
  /* Special case TODO: Isn't this an error? */
//  if (compareOrthoSegments(s1, s2))
//    return true;

  return compareOrthoLines(*(s1.line), *(s2.line)) &&
         (pointBelongsToOrthoSegment(*(s1.p1), s2) ||
          pointBelongsToOrthoSegment(*(s1.p2), s2) ||
          pointBelongsToOrthoSegment(*(s2.p1), s1) ||
          pointBelongsToOrthoSegment(*(s2.p2), s1));
}

bool checkOrthoSegmentsIntersecting(OrthoSegment s1, OrthoSegment s2) {
  Point p = getOrthoLinesIntersection(*s1.line, *s2.line);

  return pointBelongsToOrthoSegment(p, s1) &&
         pointBelongsToOrthoSegment(p, s2);
}

bool pointBelongsToOrthoSegment(Point p, OrthoSegment s) {
  if (!pointBelongsToOrthoLine(p, *(s.line)))
    return false;

  // Complicated math
  return lessEqThan((s.p1->x - p.x) * (s.p2->x - p.x), 0) &&
         lessEqThan((s.p1->y - p.y) * (s.p2->y - p.y), 0);
}

