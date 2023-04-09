#include "collisions.h"

#include <math.h>

#include "point.h"
#include "orthosegment.h"
#include "../dmath/dmath.h"

bool checkOrthoSegmentsInterlacing(OrthoSegment s1, OrthoSegment s2,
                                   bool excludeEndPoints) {
  // Special case TODO: Isn't this an error?
  //if (compareOrthoSegments(s1, s2))
  //  return true;

  return compareOrthoLines(*(s1.line), *(s2.line)) &&
         (pointBelongsToOrthoSegment(*(s1.p1), s2, excludeEndPoints) ||
          pointBelongsToOrthoSegment(*(s1.p2), s2, excludeEndPoints) ||
          pointBelongsToOrthoSegment(*(s2.p1), s1, excludeEndPoints) ||
          pointBelongsToOrthoSegment(*(s2.p2), s1, excludeEndPoints));
}



// ============================================================================

bool checkOrthoRectsSeparated(OrthoRect *rect1, OrthoRect *rect2) {
  return moreEqThan(rect1->y, rect2->y + rect2->h) ||
         lessEqThan(rect1->x + rect1->w, rect2->x) ||
         lessEqThan(rect1->y + rect1->h, rect2->y) ||
         moreEqThan(rect1->x, rect2->x + rect2->w);
}

bool checkR1OutsideR2(OrthoRect *r1, OrthoRect *r2) {
  return moreEqThan(r1->y, r2->y + r2->h) ||
         lessEqThan(r1->x + r1->w, r2->x) ||
         lessEqThan(r1->y + r1->h, r2->y) ||
         moreEqThan(r1->x, r2->x + r2->w);
}

bool checkOrthoRectsHardSeparated(OrthoRect *rect1, OrthoRect *rect2) {
  return moreThan(rect1->y, rect2->y + rect2->h) ||
         lessThan(rect1->x + rect1->w, rect2->x) ||
         lessThan(rect1->y + rect1->h, rect2->y) ||
         moreThan(rect1->x, rect2->x + rect2->w);
}

bool checkR1InsideR2(OrthoRect *r1, OrthoRect *r2) {
  return moreEqThan(r1->y, r2->y) &&
         lessEqThan(r1->x + r1->w, r2->x + r2->w) &&
         lessEqThan(r1->y + r1->h, r2->y + r2->h) &&
         moreEqThan(r1->x, r2->x);
}

bool checkR1HardInsideR2(OrthoRect *rect1, OrthoRect *rect2) {
  return moreThan(rect1->y, rect2->y) &&
         lessThan(rect1->x + rect1->w, rect2->x + rect2->w) &&
         lessThan(rect1->y + rect1->h, rect2->y + rect2->h) &&
         moreThan(rect1->x, rect2->x);
}

// Returns true if rects collide in any way
bool checkOrthoRectsCollision(OrthoRect *rect1, OrthoRect *rect2) {
  return !checkOrthoRectsHardSeparated(rect1, rect2);
}

// Returns true if rects actually intersects, not just "touch" each other
bool checkOrthoRectsGhostCollision(OrthoRect *rect1, OrthoRect *rect2) {
  return !checkOrthoRectsSeparated(rect1, rect2);
}

uint8_t getBorderCollisionMask(OrthoRect* r1, OrthoRect* r2) {
  uint8_t result = 0;
  for (uint8_t i = 0; i < 4; i++) {
    OrthoSegment edge1 = *r1->edges[i];
    for (uint8_t j = 0; j <= 2; j+=2) {
      uint8_t k = (i + j) % 4;
      OrthoSegment edge2 = *r2->edges[k];
      if (checkOrthoSegmentsInterlacing(edge1, edge2, false)) {
        result |= (uint8_t) pow(2, i);
        result |= (uint8_t) pow(2, k) * 16;
      }
    }
  }
  return result;
}

uint16_t getOrthoCollisionMask(OrthoRect* r1, OrthoRect* r2) {
  uint16_t result = 0;
  for (uint8_t i = 0; i < 4; i++) {
    OrthoSegment edge1 = *r1->edges[i];
    for (uint8_t j = 0; j <= 2; j+=2) {
      uint8_t k = (i + 1 + j) % 4;
      OrthoSegment edge2 = *r2->edges[k];
      if (checkOrthoSegmentsIntersecting(edge1, edge2, true)) {
        uint16_t m = (uint16_t) pow(2, k);
        result |= m << i * 4;
      }
    }
  }
  return result;
}


OrthoRectCollision getOrthoRectCollision(OrthoRect *r1,
                                                     OrthoRect *r2) {
  // End result
  OrthoRectCollision result = {.type = NO_COLLISION,
                                     .borderCollisionMask = 0,
                                     .orthoCollisionMask = 0};

  // No collision
  if (checkOrthoRectsHardSeparated(r1, r2)) return result;

  result.borderCollisionMask = getBorderCollisionMask(r1, r2);

  // Outer border collision
  if (result.borderCollisionMask && (checkR1OutsideR2(r1, r2) || checkR1OutsideR2(r2, r1)))
    result.type = OUTER_TOUCH;
  else // One rect inside another
  {
    // Inner border collision or BLR_R1_INSIDE_R2 collision
    if (checkR1InsideR2(r1, r2))
      result.type =
          result.borderCollisionMask ? INNER_TOUCH : INSIDE;
    // Inner border collision or BLR_R2_INSIDE_R1 collision
    else if (checkR1InsideR2(r2, r1))
      result.type =
          result.borderCollisionMask ? INNER_TOUCH : OUTSIDE;
    else // Ghost
    {
      result.orthoCollisionMask = getOrthoCollisionMask(r1, r2);
      result.type = GHOST;
    }
  }

  return result;
}


