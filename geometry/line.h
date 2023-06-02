#ifndef LINE_H
#define LINE_H

#include "point.h"

typedef struct {
  // x = x0 + y / k
  // y = y0 + k * x
  // Intersection with y axis
  double y0;
  /* k = y / x = tan(a)
     a = atan(k)
     Vertical line: k == INFINITY
     Horizontal line: k == 0 */
  double k;
  // x0 = -y0 / k
  // Intersection with x axis
  double x0;
} Line;

Line createLineFromPoints(Point p1, Point p2);
double getLineX(Line l, double y);
double getLineY(Line l, double x);

#endif
