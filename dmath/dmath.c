#include "dmath.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../debug.h"

// TODO: Handle INFINITY and NAN

void check_values(double a, double b) {
  if (a > DMATH_MAX || b > DMATH_MAX)
    WARN("dmath out of bounds");

  if (a < DMATH_MIN || b < DMATH_MIN)
    WARN("dmath out of bounds");
}

bool compare(double a, double b) {
  check_values(a, b);
  return fabs(a - b) < DMATH_EPSILON;
}

bool more_than(double a, double b) {
  check_values(a, b);
  if (compare(a, b))
    return false;
  return a > b;
}

bool less_than(double a, double b) {
  check_values(a, b);
  if (compare(a, b))
    return false;
  return a < b;
}

