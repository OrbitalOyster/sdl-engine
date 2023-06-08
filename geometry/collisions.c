#include "collisions.h"

#include <math.h>

#include "../binary.h"

#include "../debug.h"
#include "../dmath/dmath.h"
#include "line.h"
#include "orthosegment.h"
#include "point.h"

// ============================================================================

// Returns time before moving p1 will collide with static p2
double getMovingPointPointCollisionTime(Point p1, Point p2, double vx,
                                        double vy) {
  // Special case
  if (comparePoints(p1, p2))
    return 0;

#ifdef GEOMETRY_DEBUG
  double k1 = (p2.y - p1.y) / (p2.x - p1.x);
  if ((isinf(k1) && k1 < 0) || compare(p2.x, p1.x))
    k1 = INFINITY;
  double k2 = vy / vx;
  if (isinf(k2) && k2 < 0)
    k2 = INFINITY;
  if (!(isinf(k1) && isinf(k2)) && !compare(k1, k2))
    WARNF("Points are not alligned: %lf, %lf", k1, k2);
#endif

  double k = vy / vx;
  double result = (fabs(k) > 1) ? (p2.y - p1.y) / vy : (p2.x - p1.x) / vx;
  return lessThan(result, 0) ? INFINITY : result;
}

double getMovingPointOrthoSegmentIntersection(Point p, double vx, double vy,
                                              OrthoSegment s) {
  // Special case
  if (pointBelongsToOrthoSegment(p, s))
    return 0;

  // Point's velocity "vector"
  Point vp = {.x = p.x + vx, .y = p.y + vy};
  Line vl = createLineFromPoints(p, vp);

  // Point is moving parallel to the segment
  if ((s.line->isVertical && compare(vl.k, INFINITY)) ||
      (!s.line->isVertical && compare(vl.k, 0))) {
    if (!compareLineOrthoLine(vl, *s.line))
      return INFINITY;
    double t1 = getMovingPointPointCollisionTime(p, *s.p1, vx, vy);
    double t2 = getMovingPointPointCollisionTime(p, *s.p2, vx, vy);
    return lessThan(t1, t2) ? t1 : t2;
  }

  // Point is crossing segment
  double result = INFINITY;
  Point ip = getOrthoLineLineIntersection(*s.line, vl);
  if (pointBelongsToOrthoSegment(ip, s))
    result = getMovingPointPointCollisionTime(p, ip, vx, vy);

  return result;
}

// ============================================================================

// Returns true if two interlasing segments will change collision state in the 
// next moment
bool checkOrthoSegmentsAboutToDecouple(OrthoSegment s1, OrthoSegment s2,
                                       double vx, double vy) {
#ifdef GEOMETRY_DEBUG
  if (s1.line->isVertical != s2.line->isVertical)
    WARN("Segments are not parallel");
  if (!checkOrthoSegmentsInterlacing(s1, s2))
    WARN("Segments are not interlacing");
  if (compare(vx, 0) && compare(vy, 0))
    WARN("vx == 0 && vy == 0");
#endif

  // Easy case
  if ((s1.line->isVertical && !compare(vx, 0)) ||
      (!s1.line->isVertical && !compare(vy, 0)))
    return true;

  // Hard case
  if (s1.line->isVertical)
    return moreThan((s1.p1->y - s2.p1->y) * vy, 0) &&
           moreThan((s1.p1->y - s2.p2->y) * vy, 0) &&
           moreThan((s1.p2->y - s2.p1->y) * vy, 0) &&
           moreThan((s1.p2->y - s2.p2->y) * vy, 0);
  else
    return moreThan((s1.p1->x - s2.p1->x) * vx, 0) &&
           moreThan((s1.p1->x - s2.p2->x) * vx, 0) &&
           moreThan((s1.p2->x - s2.p1->x) * vx, 0) &&
           moreThan((s1.p2->x - s2.p2->x) * vx, 0);
}

double getMovingParallelOrthoSegmentsCollision(OrthoSegment s1, OrthoSegment s2,
                                               double vx, double vy) {
#ifdef GEOMETRY_DEBUG
  if (s1.line->isVertical != s2.line->isVertical)
    WARN("Segments are not parallel");
  if (checkOrthoSegmentsInterlacing(s1, s2))
    WARN("Segments are interlacing");
#endif

  double result = INFINITY;

  double intermediate[4] = {
      getMovingPointOrthoSegmentIntersection(*(s1.p1), vx, vy, s2),
      getMovingPointOrthoSegmentIntersection(*(s1.p2), vx, vy, s2),
      getMovingPointOrthoSegmentIntersection(*(s2.p1), -vx, -vy, s1),
      getMovingPointOrthoSegmentIntersection(*(s2.p2), -vx, -vy, s1),
  };

  for (int i = 0; i < 4; i++)
    if (lessThan(intermediate[i], result))
      result = intermediate[i];

  return result;
}

// ============================================================================

// NOTE: Only works for separated rects, otherwise returns RMT_CONVERGE
RelativeMovementType getOrthoRectsRelativeMovementType(OrthoRect *r1,
                                                       OrthoRect *r2,
                                                       double vx1, double vy1,
                                                       double vx2, double vy2) {
  double vx = vx1 - vx2;
  double vy = vy1 - vy2;

  if (compare(vx, 0) && compare(vy, 0))
    return RMT_NONE;

  if ((moreEqThan(r1->y, r2->y + r2->h) && moreThan(vy, 0)) ||
      (lessEqThan(r1->x + r1->w, r2->x) && lessThan(vx, 0)) ||
      (lessEqThan(r1->y + r1->h, r2->y) && lessThan(vy, 0)) ||
      (moreEqThan(r1->x, r2->x + r2->w) && moreThan(vx, 0)))
    return RMT_DIVERGE;

  // Horizontal movement
  if (compare(vy, 0)) {
    if (moreThan(r1->y, r2->y + r2->h) || lessThan(r1->y + r1->h, r2->y))
      return RMT_DIVERGE;

    if (compare(r1->edges[0]->line->xy, r2->edges[2]->line->xy) ||
        compare(r1->edges[2]->line->xy, r2->edges[0]->line->xy))
      return RMT_SLIP;
  }

  // Vertical movement
  if (compare(vx, 0)) {
    if (lessThan(r1->x + r1->w, r2->x) || moreThan(r1->x, r2->x + r2->w))
      return RMT_DIVERGE;

    if (compare(r1->edges[1]->line->xy, r2->edges[3]->line->xy) ||
        compare(r1->edges[3]->line->xy, r2->edges[1]->line->xy))
      return RMT_SLIP;
  }

  return RMT_CONVERGE;
}

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
  for (int i = 0; i < 4; i++) {
    OrthoSegment edge1 = *r1->edges[i];
    for (int j = 0; j < 2; j++) {
      int k = (i + j * 2) % 4;
      OrthoSegment edge2 = *r2->edges[k];
      if (checkOrthoSegmentsInterlacing(edge1, edge2)) {
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
  OrthoRectCollision result = {.type = NO_COLLISION,
                               .edgeCollisionMask =
                                   0 /* , .orthoCollisionMask = 0 */};

  // No collision
  if (checkOrthoRectsHardSeparated(r1, r2))
    return result;

  result.edgeCollisionMask = getEdgeCollisionMask(r1, r2);

  // Outer border collision
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
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 2; j++) {
      int k = (i + j * 2) % 4;
      OrthoSegment *s1 = r1->edges[i];
      OrthoSegment *s2 = r2->edges[k];
      if (!checkOrthoSegmentsInterlacing(*s1, *s2))
        continue;
      if (checkOrthoSegmentsAboutToDecouple(*s1, *s2, vx, vy)) {
        result |= (uint8_t)pow(2, i);
        result |= (uint8_t)(pow(2, k) * 16);
      }
    }
  return result;
}

// Returns time until next collision change
double getMovingOrthoRectsNextCollisionTime(OrthoRect *r1, OrthoRect *r2,
                                            double vx1, double vy1, double vx2,
                                            double vy2) {
  double vx = vx1 - vx2;
  double vy = vy1 - vy2;
  double result = INFINITY;
  if (compare(vx, 0) && compare(vy, 0))
    return result;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 2; j++) {
      int k = (i + j * 2) % 4;
      OrthoSegment *s1 = r1->edges[i];
      OrthoSegment *s2 = r2->edges[k];
      // Skip already colliding segments
      if (checkOrthoSegmentsInterlacing(*s1, *s2))
        continue;
      double t = getMovingParallelOrthoSegmentsCollision(*s1, *s2, vx, vy);
      if (lessThan(t, result))
        result = t;
    }
  return result;
}

// If rects collide by two opposing edges - it's a corner collision
bool isCornerCollisionMask(uint8_t mask) {
  uint8_t a = mask >> 4;
  uint8_t b = mask & 0xF;
  return a + b == 0xF;
}

// Returns time and colliding edges mask
OrthoRectCollisionChange
getMovingOrthoRectsNextCollisionChange(OrthoRect *r1, OrthoRect *r2, double vx1,
                                       double vy1, double vx2, double vy2) {
  double vx = vx1 - vx2;
  double vy = vy1 - vy2;
  OrthoRectCollisionChange result = {.time = INFINITY, .mask = 0};
  if (compare(vx, 0) && compare(vy, 0))
    return result;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 2; j++) {
      int k = (i + j * 2) % 4;
      OrthoSegment *s1 = r1->edges[i];
      OrthoSegment *s2 = r2->edges[k];
      if (checkOrthoSegmentsInterlacing(*s1, *s2))
        continue;
      double time = getMovingParallelOrthoSegmentsCollision(*s1, *s2, vx, vy);
      if (compare(time, result.time)) {
        result.mask |= (uint8_t)pow(2, i);
        result.mask |= (uint8_t)(pow(2, k) * 16);
      } else if (lessThan(time, result.time)) {
        result.time = time;
        result.mask = (uint8_t)(pow(2, i) + pow(2, k) * 16);
      }
    }
  return result;
}

