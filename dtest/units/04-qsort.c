// obj/utils/qsort.o obj/utils/dstrings.o obj/utils/lfsr.o
// 0
#include "utils/qsort.h"

#include <stdlib.h>

#include "utils/dstrings.h"
#include "utils/lfsr.h"

struct Entity {
  int a;
  unsigned int b;
  char *c;
  double d;
};

struct Entity *create_entity(int a, unsigned int b, char *c, double d) {
  struct Entity *result = calloc(1, sizeof(struct Entity));
  *result = (struct Entity){.a = a, .b = b, .c = c, .d = d};
  return result;
}

int is_sorted(struct Entity **arr, unsigned int size) {
  unsigned int prev = arr[0]->b;
  int ok = 1;
  for (unsigned int i = 1; i < size; i++) {
    if (prev > arr[i]->b)
      ok = 0;
    prev = arr[i]->b;
  }
  return ok;
}

void destroy_entities(struct Entity **arr, unsigned int size) {
  for (unsigned int i = 0; i < size; i++)
    free(arr[i]);
  free(arr);
}

int cmpr(void **arr, int i1, int i2) {
  return ((struct Entity *)arr[i1])->b < ((struct Entity *)arr[i2])->b;
}

int main() {
  DTEST_UNIT_START("Qsort test");
  struct Entity **arr;
  unsigned int size;
  unsigned int state = 1234567890;
  unsigned int taps = 61680;
  
  for (unsigned int j = 0; j < 5; j++) {
    size = lfsr(&state, taps);
    DTEST_INFOF("LSFR: %u (%s)", size, uint_to_binary(size));
    size >>= 24;
    if (size < 5) continue;
//    DTEST_INFOF("Testing array size %u (%s)", size, uint_to_binary(size));
    arr = calloc(size, sizeof(struct Entity *));
    for (unsigned int i = 0; i < size; i++) {
      unsigned int r = lfsr(&state, taps);
      arr[i] = create_entity(1, r, "hello", 0.1);
    }
    DTEST_EXPECT_FALSE(is_sorted(arr, size));
    sort((void **)arr, 0, (int) size - 1, cmpr);
    DTEST_EXPECT_TRUE(is_sorted(arr, size));
    destroy_entities(arr, size);
  }

  DTEST_UNIT_END;
}
