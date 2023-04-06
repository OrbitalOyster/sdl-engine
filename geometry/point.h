#ifndef POINT_H
#define POINT_H

#include <stdbool.h>

typedef struct {
  double x;
  double y;
} Point;

Point *createHeapPoint(double x, double y);
bool comparePoints(Point p1, Point p2);

#endif
