// obj/utils/wtree.o obj/utils/qsort.o
// 0

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
  expandWTree(tree, "abcxxx", NULL);
  expandWTree(tree, "abcxyz", &ep2);
  INFOF("Tree size: %u", tree->size);

  debugWTree(tree);
  sortWTree(tree);
  INFO("Sorted");
  debugWTree(tree);

  struct Endpoint* ep = getWTreeEndpoint(tree, "abcxyz");
  if (ep)
    INFOF("ep->c == %s", ep->c)
  else
    INFO("It's NULL");

  destroyWTree(tree);

  DTEST_UNIT_END
  return 0;
}
