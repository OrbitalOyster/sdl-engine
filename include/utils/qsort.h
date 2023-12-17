/**
 * General purpose qsort algorithm implementation
 */

#ifndef QSORT_H
#define QSORT_H

/**
 * cmpr function must return 1 when i1 and i2 elements of arr are in the WRONG
 * order; otherwise it's 0. Example cmpr:
 *
 * int entity_sort(void **arr, int i1, int i2) {
 *   return ((struct Entity *)arr[i1])->b < ((struct Entity *)arr[i2])->b;
 * }
 */

void sort(void **arr, int i1, int i2, int (*cmpr)(void **arr, int i1, int i2));

#endif /* QSORT_H */
