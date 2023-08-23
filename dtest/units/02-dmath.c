// obj/utils/dmath.o
// 0

#include "utils/dmath.h"

int main() {
  DTEST_UNIT_START("dmath");
  DTEST_EVAL_DOUBLE(.1 + .2);
  DTEST_EXPECT_TRUE(compare(.1 + .2, .3));
  DTEST_UNIT_END
  return 0;
}
