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

  expandWTree(tree, "foo", &ep1);
  expandWTree(tree, "bar", NULL);
  expandWTree(tree, "baz", &ep2);

  struct Endpoint* ep = getWTreeEndpoint(tree, "baz");
  INFOF("ep->c == %s", ep->c);

  INFOF("Tree size: %u", tree->size);
  INFOF("Word #0: %s", tree->words[0]);
  INFOF("Word #1: %s", tree->words[1]);
  INFOF("Word #2: %s", tree->words[2]);

  destroyWTree(tree);
  DTEST_UNIT_END
  return 0;
}
