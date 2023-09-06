#ifndef QSORT_H
#define QSORT_H

void sort(void **arr, int i1, int i2,
          int (*sortFunc)(void **arr, int i1, int i2));

#endif
