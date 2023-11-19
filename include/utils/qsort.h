#ifndef QSORT_H
#define QSORT_H

/**
 * sortFunc returns 1 when i1 and i2 elements of arr are in the WRONG order;
 * otherwise returns 0. Example sortFunc:
 *
 * int entitySortFunc(void **arr, int i1, int i2) {
 *   return ((struct Entity *)arr[i1])->b < ((struct Entity *)arr[i2])->b;
 * }
 */

void sort(void **arr, int i1, int i2,
          int (*sortFunc)(void **arr, int i1, int i2));

#endif /* QSORT_H */
