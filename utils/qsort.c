#include "qsort.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Sample** array;

void fillArray() {
  array = calloc(MAX, sizeof(Sample*));
  for (unsigned int i = 0; i < MAX; i++)
    array[i] = calloc(1, sizeof(Sample));

  *array[0] = (Sample){ .n = 6, .a = 100, .b = 100, .c = 100};
  *array[1] = (Sample){ .n = 5, .a = 100, .b = 100, .c = 100};
  *array[2] = (Sample){ .n = 7, .a = 100, .b = 100, .c = 100};
  *array[3] = (Sample){ .n = 4, .a = 100, .b = 100, .c = 100};
  *array[4] = (Sample){ .n = 8, .a = 100, .b = 100, .c = 100};
  *array[5] = (Sample){ .n = 3, .a = 100, .b = 100, .c = 100};
  *array[6] = (Sample){ .n = 9, .a = 100, .b = 100, .c = 100};
  *array[7] = (Sample){ .n = 2, .a = 100, .b = 100, .c = 100};
  *array[8] = (Sample){ .n = 0, .a = 100, .b = 100, .c = 100};
  *array[9] = (Sample){ .n = 1, .a = 100, .b = 100, .c = 100};
}

bool compareArr(void** arr, unsigned int i1, unsigned int i2) {
  return ((Sample*)arr[i1])->n < ((Sample*)arr[i2])->n;
}

void swap(void** arr, unsigned int i1, unsigned int i2) {
  void* tmp = arr[i1];
  arr[i1] = arr[i2];
  arr[i2] = tmp;
}

void printArray() {
  for (unsigned int i = 0; i < MAX; i++)
    printf("%i ", array[i]->n);
  printf("\n");
}

void sortTest() {
  fillArray();
  printArray();
  swap((void**)array, 8, 3);
  printArray();
  printf("compareArr %i\n", compareArr((void**)array, 9, 0));
  free(array);
}
