// obj/utils/dmath.o
// 0

#include "utils/dmath.h"

int main() {
  DTEST_UNIT_START("dmath");
  double inf = 1 / .0;
  double nan = .0 / .0;
  DTEST_EVAL_DOUBLE(.1 + .2);
  DTEST_EXPECT_TRUE(compare(.1 + .2, .3));
  DTEST_EXPECT_TRUE(compare(DMATH_EPSILON, DMATH_EPSILON / 2));
  DTEST_EXPECT_FALSE(compare(DMATH_EPSILON, DMATH_EPSILON * 2));
  DTEST_EXPECT_FALSE(compare(.1, nan));
  DTEST_EXPECT_FALSE(compare(inf, nan));
  DTEST_EXPECT_FALSE(compare(nan, nan));
  DTEST_EXPECT_TRUE(compare(inf, inf));
  DTEST_EXPECT_TRUE(compare(-inf, -inf));
  DTEST_EXPECT_FALSE(compare(inf, -inf));
  DTEST_UNIT_END
  return 0;
}
