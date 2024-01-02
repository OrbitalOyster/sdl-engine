// obj/utils/wtree.o obj/utils/dstrings.o obj/utils/qsort.o
// 0

#include <stdlib.h>

#include "utils/wtree.h"

struct Endpoint {
  int a;
  int b;
  char *c;
};

int main() {
  DTEST_UNIT_START("Word tree");
  struct WTree *wtree = create_wtree();
  struct Endpoint ep1 = {.a = 1, .b = 2, .c = "ep1"};
  struct Endpoint ep2 = {.a = 3, .b = 4, .c = "ep2"};

  DTEST_INFO("Expanding tree");
  expand_wtree(wtree, "ccc", NULL);
  expand_wtree(wtree, "abcdef", NULL);
  expand_wtree(wtree, "abc", &ep1);
  expand_wtree(wtree, "abcxyz", &ep2);
  expand_wtree(wtree, "abcxxx", NULL);
  unsigned int size = get_wtree_size(wtree);
  DTEST_EXPECT_UINT(size, 5);

  sort_wtree(wtree);
  DTEST_INFO("Tree sorted");

  struct Endpoint* ep = get_wtree_endpoint(wtree, "abc");
  DTEST_EXPECT_STRING(ep->c, "ep1");
  ep = get_wtree_endpoint(wtree, "abcxyz");
  DTEST_EXPECT_STRING(ep->c, "ep2");

  destroy_wtree(wtree);
  DTEST_UNIT_END;
}
