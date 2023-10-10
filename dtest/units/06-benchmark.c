// obj/utils/wtree.o
// 0

#include <stdio.h>
#include <stdlib.h>

#include "utils/debug.h"
#include "utils/wtree.h"

void populateTree(FILE *f, struct WTree *tree) {
  unsigned int n = 0;
  char *s = calloc(255, sizeof(char));
  int read = 0;
  do {
    read = fscanf(f, "%s\n", s);
    expandWTree(tree, s, NULL);
    n++;
  } while (read != EOF);
  free(s);
}

int main() {
  DTEST_UNIT_START("WTree benchmark");
  FILE *f = fopen("/usr/share/dict/words", "r");
  struct WTree *tree = createWTree();
  DTEST_EVAL_TIME(populateTree(f, tree));
  fclose(f);
  destroyTree(tree);
  DTEST_UNIT_END
  return 0;
}
