// obj/utils/dmath.o
// 0

#include <math.h>

#include "utils/dmath.h"

int main() {
  DTEST_UNIT_START("dmath");
  DTEST_EVAL_DOUBLE(.1 + .2);
  DTEST_EXPECT_TRUE(compare(.1 + .2, .3));
  DTEST_EXPECT_TRUE(compare(DMATH_EPSILON, DMATH_EPSILON / 2));
  DTEST_EXPECT_FALSE(compare(DMATH_EPSILON, DMATH_EPSILON * 2));
  DTEST_EXPECT_FALSE(compare(.1, INFINITY));
  DTEST_EXPECT_FALSE(compare(.1, -INFINITY));
  DTEST_EXPECT_FALSE(compare(.1, NAN));
  DTEST_EXPECT_FALSE(compare(INFINITY, NAN));
  DTEST_EXPECT_FALSE(compare(NAN, NAN));
  DTEST_EXPECT_TRUE(compare(INFINITY, INFINITY));
  DTEST_EXPECT_TRUE(compare(-INFINITY, -INFINITY));
  DTEST_EXPECT_FALSE(compare(INFINITY, -INFINITY));
  DTEST_UNIT_END
  return 0;
}
