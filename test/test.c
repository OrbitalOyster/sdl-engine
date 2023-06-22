#include "test.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../dmath/dmath.h"
#include "../utils/qsort.h"

// dmath
/*===========================================================================*/
bool compareTest() {
  double a = 0.1;
  double b = 0.2;
  bool result = compare(a + b, 0.3);
  printf("compare(a + b, 0.3) -> %i\n", result);
  return result;
}

// qsort
/*===========================================================================*/
#define MAX 10

typedef struct {
  int n;
  int a;
  int b;
  int c;
} Sample;

Sample** array;

bool qLessThan(void** arr, int i1, int i2) {
  return ((Sample*)arr[i1])->n < ((Sample*)arr[i2])->n;
}

void fillArray() {
  array = calloc(MAX, sizeof(Sample*));
  for (int i = 0; i < MAX; i++)
    array[i] = calloc(1, sizeof(Sample));

  *array[0] = (Sample){ .n = 9, .a = 100, .b = 100, .c = 100};
  *array[1] = (Sample){ .n = 8, .a = 100, .b = 100, .c = 100};
  *array[2] = (Sample){ .n = 7, .a = 100, .b = 100, .c = 100};
  *array[3] = (Sample){ .n = 6, .a = 100, .b = 100, .c = 100};
  *array[4] = (Sample){ .n = 5, .a = 100, .b = 100, .c = 100};
  *array[5] = (Sample){ .n = 4, .a = 100, .b = 100, .c = 100};
  *array[6] = (Sample){ .n = 4, .a = 100, .b = 100, .c = 100};
  *array[7] = (Sample){ .n = 5, .a = 100, .b = 100, .c = 100};
  *array[8] = (Sample){ .n = 6, .a = 100, .b = 100, .c = 100};
  *array[9] = (Sample){ .n = 7, .a = 100, .b = 100, .c = 100};
}

void printArray() {
  for (int i = 0; i < MAX; i++)
    printf("%i ", array[i]->n);
  printf("\n");
}

void sortTest() {
  fillArray();
  printArray();
  sort((void**)array, 0, 9, qLessThan);
  printArray();
  free(array);
}
/*===========================================================================*/

int test() {
  compareTest();
  sortTest();
  return 0;
}

