#include "dmath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"

// TODO: Handle NAN

void checkValues(double a, double b) {
  if (isnan(a) || isnan(b))
    WARN("Unhandled NAN");

  if (a > DMATH_MAX || b > DMATH_MAX)
    WARNF("dmath out of bounds: %lf, %lf", a, b);

  if (a < DMATH_MIN || b < DMATH_MIN)
    WARNF("dmath out of bounds: %lf, %lf", a, b);
}

bool compare(double a, double b) {
  if (isfinite(a) && isfinite(b)) {
    checkValues(a, b);
    return fabs(a - b) < DMATH_EPSILON;
  } else
    return isinf(a) && isinf(b) && !((a < 0) ^ (b < 0));
}

bool compareFinite(double a, double b) {
  checkValues(a, b);
  return fabs(a - b) < DMATH_EPSILON;
}

bool moreThan(double a, double b) {
  if (isfinite(a) && isfinite(b) && compareFinite(a, b))
    return false;
  return a > b;
}

bool lessThan(double a, double b) {
  if (isfinite(a) && isfinite(b) && compareFinite(a, b))
    return false;
  return a < b;
}

bool moreEqThan(double a, double b) {
  if (compare(a, b))
    return true;
  return a > b;
}

bool lessEqThan(double a, double b) {
  if (compare(a, b))
    return true;
  return a < b;
}

