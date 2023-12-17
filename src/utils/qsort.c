#include "utils/qsort.h"

static void swap(void **arr, int i1, int i2) {
  void *tmp = arr[i1];
  arr[i1] = arr[i2];
  arr[i2] = tmp;
}

static int get_pivot(void **arr, int i1, int i2) {
  // Insert any pivot-finding implementation
  (void)(arr && i1 && i2);
  return i2;
}

static int get_l_ind(void **arr, int pivot_ind, int i1, int i2,
                     int (*cmpr)(void **arr, int i1, int i2)) {
  int i = i1;
  while (i <= i2)
    if (!cmpr(arr, i, pivot_ind))
      return i;
    else
      i++;
  return i;
}

static int get_r_ind(void **arr, int pivot_ind, int i1, int i2,
                     int (*cmpr)(void **arr, int i1, int i2)) {
  int i = i2;
  while (i >= i1)
    if (cmpr(arr, i, pivot_ind))
      return i;
    else
      i--;
  return i;
}

static void step(void **arr, int i1, int i2,
                 int (*cmpr)(void **arr, int i1, int i2)) {
  // Nothing to sort
  if (i1 >= i2)
    return;
  // Get pivot, move it out of the way
  int pivot_ind = get_pivot(arr, i1, i2);
  swap(arr, i2, pivot_ind);
  pivot_ind = i2;
  // Set new bounds
  int l_ind = i1;
  int r_ind = i2 - 1;
  while (l_ind <= r_ind) {
    l_ind = get_l_ind(arr, pivot_ind, i1, i2 - 1, cmpr);
    r_ind = get_r_ind(arr, pivot_ind, i1, i2 - 1, cmpr);
    if (l_ind <= r_ind)
      swap(arr, l_ind, r_ind);
  }
  // Bring pivot back, repeat the process
  swap(arr, l_ind, pivot_ind);
  step(arr, i1, l_ind - 1, cmpr);
  step(arr, l_ind + 1, i2, cmpr);
}

void sort(void **arr, int i1, int i2, int (*cmpr)(void **arr, int i1, int i2)) {
  step(arr, i1, i2, cmpr);
}
