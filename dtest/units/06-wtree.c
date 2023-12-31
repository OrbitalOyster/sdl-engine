// obj/utils/wtree.o obj/utils/dstrings.o obj/utils/qsort.o
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
  struct WTree *wtree = create_wtree();
  struct Endpoint ep1 = {.a = 1, .b = 2, .c = "ep1"};
  struct Endpoint ep2 = {.a = 3, .b = 4, .c = "ep2"};

  expand_wtree(wtree, "ccc", NULL);
  expand_wtree(wtree, "abcdef", NULL);
  expand_wtree(wtree, "abc", &ep1);
  expand_wtree(wtree, "abcxyz", &ep2);
  expand_wtree(wtree, "abcxxx", NULL);
  unsigned int size = get_wtree_size(wtree);
  INFOF("Tree size: %u", size);

  sort_wtree(wtree);
  INFO("Sorted");

  char **words = get_wtree_words(wtree);
  for (unsigned int i = 0; i < size; i++)
    INFO2F("%s", words[i]);
  for (unsigned int i = 0; i < size; i++)
    free(words[i]);
  free(words);

  struct Endpoint* ep = get_wtree_endpoint(wtree, "abc");
  if (ep)
    INFOF("ep->c == %s", ep->c)
  else
    INFO("It's NULL");

  destroy_wtree(wtree);

  DTEST_UNIT_END;
}
