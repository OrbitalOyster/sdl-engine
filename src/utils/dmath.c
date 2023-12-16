#include "utils/dmath.h"

#include <math.h>

#include "utils/debug.h"

static void check_values(double a, double b) {
  if (a > DMATH_MAX || b > DMATH_MAX)
    WARNF("dmath out of bounds: %lf, %lf", a, b);

  if (a < DMATH_MIN || b < DMATH_MIN)
    WARNF("dmath out of bounds: %lf, %lf", a, b);
}

int compare(double a, double b) {
  if (isfinite(a) && isfinite(b)) {
    check_values(a, b);
    return fabs(a - b) < DMATH_EPSILON;
  } else
    return isinf(a) && isinf(b) && !((a < 0) ^ (b < 0));
}

static int compare_finite(double a, double b) {
  check_values(a, b);
  return fabs(a - b) < DMATH_EPSILON;
}

int more(double a, double b) {
  if (isfinite(a) && isfinite(b) && compare_finite(a, b))
    return 0;
  return a > b;
}

int less(double a, double b) {
  if (isfinite(a) && isfinite(b) && compare_finite(a, b))
    return 0;
  return a < b;
}

int more_eq(double a, double b) {
  if (compare(a, b))
    return 1;
  return a > b;
}

int less_eq(double a, double b) {
  if (compare(a, b))
    return 1;
  return a < b;
}
