// obj/utils/wtree.o
// 0

#include "utils/wtree.h"
#include "utils/debug.h"

struct Endpoint {
  int a;
  int b;
};

int main() {
  DTEST_UNIT_START("Word tree");
  struct WTree *tree = createWTree();
  struct Endpoint ep1 = {.a = 1, .b = 2};
  struct Endpoint ep2 = {.a = 3, .b = 4};
  expandWTree(tree, "foo", NULL);
  expandWTree(tree, "bar", &ep1);
  expandWTree(tree, "baz", &ep2);

  struct Endpoint* ep = getWTreeEndpoint(tree, "foo");
  INFOF("ep->b == %i", ep->b);

  destroyTree(tree);
  DTEST_UNIT_END
  return 0;
}
