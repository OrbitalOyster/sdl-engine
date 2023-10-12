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
char chars[62];

struct Endpoint {
  char *msg;
};

struct Endpoint *oyster;

void populateChars() {
  unsigned int n = 0;
  for (char c = '0'; c <= '9'; c++)
    chars[n++] = c;
  for (char c = 'a'; c <= 'z'; c++)
    chars[n++] = c;
  for (char c = 'A'; c <= 'Z'; c++)
    chars[n++] = c;
}

char getRandomChar() {
  return chars[getNextLFSR(&state, taps) % 62];
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
    expandWTree(tree, keys[i], NULL);
  }
}

void destroyKeys() {
  for (int i = 0; i < NKEYS; i++)
    free(keys[i]);
  free(keys);
}

int main() {
  DTEST_UNIT_START("WTree benchmark");
  DTEST_EVAL_TIME(populateChars());
  DTEST_EVAL_TIME(populateKeys());
  struct WTree *tree = createWTree();
  DTEST_EVAL_TIME(populateTree(tree));
  destroyTree(tree);
  destroyKeys();
  DTEST_UNIT_END
  return 0;
}
