/*
 * "d" stands for "dullard"
 */

#ifndef DMATH_H
#define DMATH_H

#define DMATH_EPSILON 0.00001
#define DMATH_MAX 1000000000.0
#define DMATH_MIN -1000000000.0

int compare(double a, double b);
int more(double a, double b);
int less(double a, double b);
int more_eq(double a, double b);
int less_eq(double a, double b);

#endif /* DMATH_H */
