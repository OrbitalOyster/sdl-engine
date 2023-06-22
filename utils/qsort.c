#include "qsort.h"

void swap(void **arr, int i1, int i2) {
  void *tmp = arr[i1];
  arr[i1] = arr[i2];
  arr[i2] = tmp;
}

int getPivot(void **arr, int i1, int i2) {
  if (arr && i1) {
  };
  return i2;
}

int getLeftInd(void **arr, int pivotInd, int i1, int i2,
               bool (*sortFunc)(void **arr, int i1, int i2)) {
  int i = i1;
  while (i <= i2)
    if (!sortFunc(arr, i, pivotInd))
      return i;
    else
      i++;
  return i;
}

int getRightInd(void **arr, int pivotInd, int i1, int i2,
                bool (*sortFunc)(void **arr, int i1, int i2)) {
  int i = i2;
  while (i >= i1)
    if (sortFunc(arr, i, pivotInd))
      return i;
    else
      i--;
  return i;
}

void step(void **arr, int i1, int i2,
          bool (*sortFunc)(void **arr, int i1, int i2)) {
  if (i1 >= i2)
    return;
  if (i1 + 1 == i2) {
    if (!sortFunc(arr, i1, i2))
      swap(arr, i1, i2);
    return;
  }

  int pivotInd = getPivot(arr, i1, i2);
  swap(arr, i2, pivotInd);

  int leftInd = i1;
  int rightInd = i2 - 1;

  bool done = false;
  while (!done) {
    leftInd = getLeftInd(arr, pivotInd, i1, i2 - 1, sortFunc);
    rightInd = getRightInd(arr, pivotInd, i1, i2 - 1, sortFunc);
    if (leftInd > rightInd) {
      swap(arr, leftInd, pivotInd);
      done = true;
    } else
      swap(arr, leftInd, rightInd);
  }
  step(arr, i1, leftInd - 1, sortFunc);
  step(arr, leftInd + 1, i2, sortFunc);
}

void sort(void **arr, int i1, int i2,
          bool (*sortFunc)(void **arr, int i1, int i2)) {
  step(arr, i1, i2, sortFunc);
}

