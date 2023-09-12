// obj/utils/wtree.o
// 0

#include "utils/wtree.h"

int main() {
  DTEST_UNIT_START("Word tree");
  struct WTree *tree = createWTree();
  expandWTree(tree, "foo");
  expandWTree(tree, "bar");
  expandWTree(tree, "baz");
  DTEST_UNIT_END
  return 0;
}
