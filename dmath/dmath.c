#include "dmath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"

// TODO: Handle INFINITY and NAN

void checkValues(double a, double b) {
  if (a > DMATH_MAX || b > DMATH_MAX)
    WARN("dmath out of bounds");

  if (a < DMATH_MIN || b < DMATH_MIN)
    WARN("dmath out of bounds");
}

bool compare(double a, double b) {
  checkValues(a, b);
  return fabs(a - b) < DMATH_EPSILON;
}

bool moreThan(double a, double b) {
  checkValues(a, b);
  if (compare(a, b))
    return false;
  return a > b;
}

bool lessThan(double a, double b) {
  checkValues(a, b);
  if (compare(a, b))
    return false;
  return a < b;
}

bool moreEqThan(double a, double b) {
  checkValues(a, b);
  if (compare(a, b))
    return true;
  return a > b;
}

bool lessEqThan(double a, double b) {
  checkValues(a, b);
  if (compare(a, b))
    return true;
  return a < b;
}

