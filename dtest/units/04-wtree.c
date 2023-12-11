// obj/utils/wtree.o obj/utils/qsort.o
// 0

#include <stdlib.h>

#include "utils/wtree.h"
#include "utils/debug.h"

struct Endpoint {
  int a;
  int b;
  char *c;
};

int main() {
  DTEST_UNIT_START("Word tree");
  struct WTree *tree = createWTree();
  struct Endpoint ep1 = {.a = 1, .b = 2, .c = "ep1"};
  struct Endpoint ep2 = {.a = 3, .b = 4, .c = "ep2"};

  expandWTree(tree, "ccc", NULL);
  expandWTree(tree, "abcdef", NULL);
  expandWTree(tree, "abc", &ep1);
  expandWTree(tree, "abcxyz", &ep2);
  expandWTree(tree, "abcxxx", NULL);
  unsigned int size = getWTreeSize(tree);
  INFOF("Tree size: %u", size);

  sortWTree(tree);
  INFO("Sorted");

  char **words = getWTreeWords(tree);
  for (unsigned int i = 0; i < size; i++)
    INFO2F("%s", words[i]);
  for (unsigned int i = 0; i < size; i++)
    free(words[i]);
  free(words);

  struct Endpoint* ep = getWTreeEndpoint(tree, "abc");
  if (ep)
    INFOF("ep->c == %s", ep->c)
  else
    INFO("It's NULL");

  destroyWTree(tree);

  DTEST_UNIT_END
  return 0;
}
