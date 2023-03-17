/*
 * "d" stands for "dummy"
 */

#ifndef _DMATH_H_
#define _DMATH_H_

#include <stdbool.h>

#define DMATH_EPSILON 0.00001
#define DMATH_MAX  1000000000.0
#define DMATH_MIN -1000000000.0

bool compare(double a, double b);
bool more_than(double a, double b);
bool less_than(double a, double b);

#endif

