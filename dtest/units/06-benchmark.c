// obj/utils/wtree.o
// 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/wtree.h"

struct Endpoint {
  char *msg;
};

struct Endpoint *oyster;

void populateTree(FILE *f, struct WTree *tree) {
  unsigned int n = 0;
  char *s = calloc(255, sizeof(char));
  int read = 0;
  do {
    read = fscanf(f, "%s\n", s);
    if (strcmp(s, "oyster") == 0)
      expandWTree(tree, s, oyster);
    else
      expandWTree(tree, s, NULL);
    n++;
  } while (read != EOF);
  free(s);
}

int main() {
  DTEST_UNIT_START("WTree benchmark");
  oyster = calloc(1, sizeof(struct Endpoint));
  *oyster = (struct Endpoint){.msg = "oyster"};
  FILE *f = fopen("/usr/share/dict/words", "r");
  struct WTree *tree = createWTree();
  DTEST_EVAL_TIME(populateTree(f, tree));
  struct Endpoint *lookup;
  DTEST_EVAL_TIME(lookup = getWTreeEndpoint(tree, "oyster"));
  DTEST_EXPECT_TRUE(lookup != NULL);
  INFOF("Lookup: %s", lookup->msg);
  fclose(f);
  destroyTree(tree);
  free(oyster);
  DTEST_UNIT_END
  return 0;
}
