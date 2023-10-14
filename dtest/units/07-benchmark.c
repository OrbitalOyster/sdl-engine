// obj/utils/wtree.o obj/utils/lfsr.o
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

#define NKEYS 1000000
#define KEYLENGTH 16

char **keys;

struct Endpoint {
  char *msg;
};

struct Endpoint *oyster;

char getRandomChar() {
  return WTREE_CHARS[getNextLFSR(&state, taps) & (WTREE_CHARS_NUMBER - 1)];
}

void populateKeys() {
  keys = calloc(NKEYS, sizeof(char*));
  for (int i = 0; i < NKEYS; i++) {
    keys[i] = calloc(KEYLENGTH, sizeof(char));
    for (int j = 0; j < KEYLENGTH - 1; j++)
      keys[i][j] = getRandomChar();
    keys[i][KEYLENGTH - 1] = '\0';
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
  return e->msg;
}

int main() {
  DTEST_UNIT_START("WTree benchmark");
  oyster = calloc(1, sizeof(struct Endpoint));
  *oyster = (struct Endpoint){.msg = "oyster"};
  DTEST_EVAL_TIME(populateKeys());
  struct WTree *tree = createWTree();
  DTEST_EVAL_TIME(populateTree(tree));
  char *msg;
  DTEST_EVAL_TIME(msg = lookup(tree));
  INFOF("Msg: %s", msg);
  destroyTree(tree);
  destroyKeys();
  DTEST_UNIT_END
  return 0;
}
