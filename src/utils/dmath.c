#include "utils/dmath.h"

#include <math.h>

#include "utils/debug.h"

static void checkValues(double a, double b) {
  if (a > DMATH_MAX || b > DMATH_MAX)
    WARNF("dmath out of bounds: %lf, %lf", a, b);

  if (a < DMATH_MIN || b < DMATH_MIN)
    WARNF("dmath out of bounds: %lf, %lf", a, b);
}

int compare(double a, double b) {
  if (isfinite(a) && isfinite(b)) {
    checkValues(a, b);
    return fabs(a - b) < DMATH_EPSILON;
  } else
    return isinf(a) && isinf(b) && !((a < 0) ^ (b < 0));
}

static int compareFinite(double a, double b) {
  checkValues(a, b);
  return fabs(a - b) < DMATH_EPSILON;
}

int moreThan(double a, double b) {
  if (isfinite(a) && isfinite(b) && compareFinite(a, b))
    return 0;
  return a > b;
}

int lessThan(double a, double b) {
  if (isfinite(a) && isfinite(b) && compareFinite(a, b))
    return 0;
  return a < b;
}

int moreEqThan(double a, double b) {
  if (compare(a, b))
    return 1;
  return a > b;
}

int lessEqThan(double a, double b) {
  if (compare(a, b))
    return 1;
  return a < b;
}
