#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dmath.h"

void check_values(double a, double b) {
  if (a > DMATH_MAX || b > DMATH_MAX) {
    fprintf(stderr, "dmath error");
    exit(1);
  }

  if (a < DMATH_MIN || b < DMATH_MIN) {
    fprintf(stderr, "dmath error");
    exit(1);
  }
}

bool compare(double a, double b) {
  check_values(a, b);
  if (a == b) return true;
  return fabs(a - b) < DMATH_EPSILON;
}

bool more_than(double a, double b) {
  check_values(a, b);
  if (compare(a, b)) return false;
  return a > b;
}

bool less_than(double a, double b) {
  check_values(a, b);
  if (compare(a, b)) return false;
  return a < b;
}

