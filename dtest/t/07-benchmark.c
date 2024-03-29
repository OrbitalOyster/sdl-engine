// obj/utils/wtree.o obj/utils/lfsr.o obj/utils/qsort.o
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

#define NKEYS 5
#define KEYLENGTH 16

char **keys;

struct Endpoint {
  char *msg;
};

struct Endpoint *oyster;

char getRandomChar() {
  return WTREE_CHARS[lfsr(&state, taps) & (WTREE_CHARS_NUMBER - 1)];
}

void populateKeys() {
//  INFO2("");
  keys = calloc(NKEYS, sizeof(char*));
  for (int i = 0; i < NKEYS; i++) {
    keys[i] = calloc(KEYLENGTH, sizeof(char));
    for (int j = 0; j < KEYLENGTH - 1; j++)
      keys[i][j] = getRandomChar();
    keys[i][KEYLENGTH - 1] = '\0';
//    INFO2F("%s", keys[i]);
  }
}

void populateTree(struct WTree *tree) {
  for (int i = 0; i < NKEYS; i++) {
    if (i == NKEYS - 1)
      expandWTree(tree, keys[i], oyster);
    else
      expandWTree(tree, keys[i], NULL);
  }
}

void destroyKeys() {
  for (int i = 0; i < NKEYS; i++)
    free(keys[i]);
  free(keys);
}

char *lookup(struct WTree *tree) {
  struct Endpoint *e;
  for (int i = 0; i < NKEYS; i++)
    e = getWTreeEndpoint(tree, keys[i]);
  if (e)
    return e->msg;
  else
    return NULL;
}

int main() {
  DTEST_UNIT_START("WTree benchmark");
  DTEST_EVAL_TIME(populateKeys());
  struct WTree *tree = createWTree();
  DTEST_EVAL_TIME(populateTree(tree));
  unsigned int size = getWTreeSize(tree);
  DTEST_EVAL_TIME(sortWTree(tree));
  char **words;
  DTEST_EVAL_TIME(words = getWTreeWords(tree));
  for (unsigned int i = 0; i < size; i++) {
    INFOF("%s", words[i]);
    free(words[i]);
  }
  free(words);
  DTEST_EVAL_TIME(lookup(tree));
  free(oyster);
  destroyWTree(tree);
  destroyKeys();
  DTEST_UNIT_END;
}
