// obj/utils/qsort.o
// 0
#include <stdio.h>
#include <stdlib.h>

#include "utils/qsort.h"

#define ARR_SIZE 10 

struct Entity {
  int a;
  int b;
  char *c;
  double d;
};

struct Entity *createEntity(int a, int b, char *c, double d) {
  struct Entity *result = calloc(1, sizeof(struct Entity));
  *result = (struct Entity){.a = a, .b = b, .c = c, .d = d};
  return result;
}

void fillEntities(struct Entity **arr) {
  arr[0] = createEntity(00, 7, "0", .123);
  arr[1] = createEntity(10, 2, "0", .123);
  arr[2] = createEntity(20, 7, "0", .123);
  arr[3] = createEntity(30, 3, "0", .123);
  arr[4] = createEntity(40, 2, "0", .123);
  arr[5] = createEntity(50, 6, "0", .123);
  arr[6] = createEntity(60, 8, "0", .123);
  arr[7] = createEntity(70, 9, "0", .123);
  arr[8] = createEntity(80, 5, "0", .123);
  arr[9] = createEntity(90, 4, "0", .123);
}

int checkEntitiesSorted(struct Entity **arr) {
  int prev = arr[0] -> b;
  int ok = 1;
  for (int i = 1; i < ARR_SIZE; i++) {
    if (prev > arr[i] -> b) ok = 0;
    prev = arr[i] -> b;
  }
  return ok;
}

void destroyEntities(struct Entity **arr) {
  for (int i = 0; i < ARR_SIZE; i++)
    free(arr[i]);
  free(arr);
}

int entitySortFunc(void **arr, int i1, int i2) {
  return ((struct Entity *)arr[i1])->b < ((struct Entity *)arr[i2])->b;
}

int main() {
  DTEST_UNIT_START("Qsort test");
  struct Entity **entities = calloc(ARR_SIZE, sizeof(struct Entity *));
  fillEntities(entities);
  DTEST_EXPECT_FALSE(checkEntitiesSorted(entities));
  sort((void **)entities, 0, ARR_SIZE - 1, entitySortFunc);
  DTEST_EXPECT_TRUE(checkEntitiesSorted(entities));
  DTEST_UNIT_END
  destroyEntities(entities);
  return 0;
}
