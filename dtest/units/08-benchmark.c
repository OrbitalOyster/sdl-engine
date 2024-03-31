// obj/utils/wtree.o obj/utils/lfsr.o obj/utils/qsort.o obj/utils/dstrings.o
// 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/lfsr.h"
#include "utils/wtree.h"

const LFSR_TYPE taps = 61680; // 1111000011110000
const LFSR_TYPE initialState = 12345;
LFSR_TYPE state = initialState;

#define NKEYS 5000000u
#define KEYLENGTH 16u

char **keys;

struct Endpoint {
  char *msg;
};

struct Endpoint *oyster;

char get_random_char() {
  return WTREE_CHARS[lfsr(&state, taps) & (WTREE_CHARS_NUMBER - 1)];
}

void populate_keys() {
  keys = calloc(NKEYS, sizeof(char *));
  for (unsigned int i = 0; i < NKEYS; i++) {
    keys[i] = calloc(KEYLENGTH, sizeof(char));
    for (unsigned int j = 0; j < KEYLENGTH - 1; j+=5) {
      uint32_t rnd32 = lfsr(&state, taps);

      keys[i][j] = WTREE_CHARS[rnd32 & 63];
      rnd32 >>= 6;
      keys[i][j+1] = WTREE_CHARS[rnd32 & 63];
      rnd32 >>= 6;
      keys[i][j+2] = WTREE_CHARS[rnd32 & 63];
      rnd32 >>= 6;
      keys[i][j+3] = WTREE_CHARS[rnd32 & 63];
      rnd32 >>= 6;
      keys[i][j+4] = WTREE_CHARS[rnd32 & 63];

    }
    keys[i][KEYLENGTH - 1] = '\0';
  }
}

void populate_wtree(struct WTree *tree) {
  for (unsigned int i = 0; i < NKEYS; i++) {
    if (i == NKEYS - 1)
      expand_wtree(tree, keys[i], oyster);
    else
      expand_wtree(tree, keys[i], NULL);
  }
}

void destroy_keys() {
  for (unsigned int i = 0; i < NKEYS; i++)
    free(keys[i]);
  free(keys);
}

char *lookup(struct WTree *tree) {
  struct Endpoint *e;
  for (unsigned int i = 0; i < NKEYS; i++)
    e = get_wtree_endpoint(tree, keys[i]);
  if (e)
    return e->msg;
  else
    return NULL;
}

int main() {
  DTEST_UNIT_START("WTree benchmark");
  DTEST_EVAL_TIME(populate_keys());
  struct WTree *tree = create_wtree();
  DTEST_EVAL_TIME(populate_wtree(tree));
  unsigned int size = get_wtree_size(tree);
  DTEST_EVAL_TIME(sort_wtree(tree));
  char **words;
  DTEST_EVAL_TIME(words = get_wtree_words(tree));
  for (unsigned int i = 0; i < size; i++) {
    INFOF("%s", words[i]);
    free(words[i]);
  }
  free(words);
  DTEST_EVAL_TIME(lookup(tree));
  free(oyster);
  destroy_wtree(tree);
  destroy_keys();
  DTEST_UNIT_END;
}
