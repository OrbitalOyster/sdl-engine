#include "collisions.h"

#include <math.h>

#include "../debug.h"
#include "../dmath/dmath.h"
#include "orthosegment.h"
#include "point.h"

// ============================================================================

// Returns true if two interlasing segments will change collision state in next
// moment
bool checkOrthoSegmentsAboutToDecouple(OrthoSegment s1, OrthoSegment s2,
                                       double vx, double vy) {
#ifdef GEOMETRY_DEBUG
  if (s1.line->isVertical != s2.line->isVertical)
    WARN("Segments are not parallel");
  if (!checkOrthoSegmentsInterlacing(s1, s2, false))
    WARN("Segments are not interlacing");
  if (compare(vx, 0) && compare(vy, 0))
    printf("vx == 0 && vy == 0");
#endif

  if ((s1.line->isVertical && !compare(vx, 0)) ||
      (!s1.line->isVertical && !compare(vy, 0)))
    return true;

  if (s1.line->isVertical)
    return (s1.p1->y - s2.p1->y) * vy >= 0 && (s1.p1->y - s2.p2->y) * vy >= 0 &&
           (s1.p2->y - s2.p1->y) * vy >= 0 && (s1.p2->y - s2.p2->y) * vy >= 0;
  else
    return (s1.p1->x - s2.p1->x) * vx >= 0 && (s1.p1->x - s2.p2->x) * vx >= 0 &&
           (s1.p2->x - s2.p1->x) * vx >= 0 && (s1.p2->x - s2.p2->x) * vx >= 0;
}

// ============================================================================

bool checkOrthoRectsSeparated(OrthoRect *r1, OrthoRect *r2) {
  return moreEqThan(r1->y, r2->y + r2->h) || lessEqThan(r1->x + r1->w, r2->x) ||
         lessEqThan(r1->y + r1->h, r2->y) || moreEqThan(r1->x, r2->x + r2->w);
}

bool checkOrthoRectsHardSeparated(OrthoRect *r1, OrthoRect *r2) {
  return moreThan(r1->y, r2->y + r2->h) || lessThan(r1->x + r1->w, r2->x) ||
         lessThan(r1->y + r1->h, r2->y) || moreThan(r1->x, r2->x + r2->w);
}

bool checkR1InsideR2(OrthoRect *r1, OrthoRect *r2) {
  return moreEqThan(r1->y, r2->y) && lessEqThan(r1->x + r1->w, r2->x + r2->w) &&
         lessEqThan(r1->y + r1->h, r2->y + r2->h) && moreEqThan(r1->x, r2->x);
}

bool checkR1HardInsideR2(OrthoRect *r1, OrthoRect *r2) {
  return moreThan(r1->y, r2->y) && lessThan(r1->x + r1->w, r2->x + r2->w) &&
         lessThan(r1->y + r1->h, r2->y + r2->h) && moreThan(r1->x, r2->x);
}

uint8_t getEdgeCollisionMask(OrthoRect *r1, OrthoRect *r2) {
  uint8_t result = 0;
  for (uint8_t i = 0; i < 4; i++) {
    OrthoSegment edge1 = *r1->edges[i];
    for (uint8_t j = 0; j < 2; j++) {
      uint8_t k = (uint8_t)(i + j * 2) % 4;
      OrthoSegment edge2 = *r2->edges[k];
      if (checkOrthoSegmentsInterlacing(edge1, edge2, false)) {
        result |= (uint8_t)pow(2, i);
        result |= (uint8_t)(pow(2, k) * 16);
      }
    }
  }
  return result;
}

/*
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
*/

OrthoRectCollision getOrthoRectCollision(OrthoRect *r1, OrthoRect *r2) {
  // End result
  OrthoRectCollision result = {
      .type = NO_COLLISION, .edgeCollisionMask = 0, .orthoCollisionMask = 0};

  // No collision
  if (checkOrthoRectsHardSeparated(r1, r2))
    return result;

  result.edgeCollisionMask = getEdgeCollisionMask(r1, r2);

  // Outer border collision
  //  if (result.edgeCollisionMask && (checkOrthoRectsSeparated(r1, r2) ||
  //  checkOrthoRectsSeparated(r2, r1)))
  if (result.edgeCollisionMask && checkOrthoRectsSeparated(r1, r2))
    result.type = OUTER_TOUCH;
  else // One rect inside another
  {
    if (checkR1InsideR2(r1, r2))
      result.type = result.edgeCollisionMask ? INNER_TOUCH : INSIDE;
    else if (checkR1InsideR2(r2, r1))
      result.type = result.edgeCollisionMask ? INNER_TOUCH : OUTSIDE;
    else // Ghost
    {
      // result.orthoCollisionMask = getOrthoCollisionMask(r1, r2);
      result.type = GHOST;
    }
  }

  return result;
}

uint8_t getMovingOrthoRectsImmediateCollisionChange(OrthoRect *r1,
                                                    OrthoRect *r2, double vx1,
                                                    double vy1, double vx2,
                                                    double vy2) {
  double vx = vx1 - vx2;
  double vy = vy1 - vy2;
  uint8_t result = 0;
  if (compare(vx, 0) && compare(vy, 0))
    return result;
  for (uint8_t i = 0; i < 4; i++)
    for (uint8_t j = 0; j < 2; j++) {
      uint8_t k = (uint8_t)(i + j * 2) % 4;
      OrthoSegment *s1 = r1->edges[i];
      OrthoSegment *s2 = r2->edges[k];
      if (!checkOrthoSegmentsInterlacing(*s1, *s2, false))
        continue;
      if (checkOrthoSegmentsAboutToDecouple(*s1, *s2, vx, vy)) {
        result |= (uint8_t)pow(2, i);
        result |= (uint8_t)(pow(2, k) * 16);
      }
    }
  return result;
}

