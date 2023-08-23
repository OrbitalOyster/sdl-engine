/*
 * "d" stands for "dullard"
 */

#ifndef DMATH_H
#define DMATH_H

#define DMATH_EPSILON 0.00001
#define DMATH_MAX  1000000000.0
#define DMATH_MIN -1000000000.0

int compare(double a, double b);
int moreThan(double a, double b);
int lessThan(double a, double b);
int moreEqThan(double a, double b);
int lessEqThan(double a, double b);

#endif
