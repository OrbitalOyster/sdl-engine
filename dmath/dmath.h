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
bool moreThan(double a, double b);
bool lessThan(double a, double b);
bool moreEqThan(double a, double b);
bool lessEqThan(double a, double b);

#endif
