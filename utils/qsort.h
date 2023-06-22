#ifndef QSORT_H
#define QSORT_H

#include <stdbool.h>

void sort(void **arr, int i1, int i2, bool(*sortFunc)(void** arr, int i1, int i2));

#endif
