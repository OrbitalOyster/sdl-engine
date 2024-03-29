// obj/utils/dmath.o
// 0

#include <math.h>

#include "utils/dmath.h"

int main() {
  DTEST_UNIT_START("dmath");

  DTEST_INFO("Basic stuff");
  DTEST_EVAL_DOUBLE(.1 + .2);
  DTEST_EXPECT_TRUE(compare(.1 + .2, .3));
  DTEST_EXPECT_TRUE(more(1 + DMATH_EPSILON, 1));
  DTEST_EXPECT_FALSE(more(1 + DMATH_EPSILON / 2, 1));
  DTEST_EXPECT_TRUE(less(1 - DMATH_EPSILON * 2, 1));
  DTEST_EXPECT_FALSE(less(1 - DMATH_EPSILON / 2, 1));

  DTEST_INFO("Epsilon check");
  DTEST_EXPECT_TRUE(compare(DMATH_EPSILON, DMATH_EPSILON / 2));
  DTEST_EXPECT_FALSE(compare(DMATH_EPSILON, DMATH_EPSILON * 2));

  DTEST_INFO("Infinity handling");
  DTEST_EXPECT_FALSE(compare(.1, INFINITY));
  DTEST_EXPECT_FALSE(compare(.1, -INFINITY));
  DTEST_EXPECT_TRUE(compare(INFINITY, INFINITY));
  DTEST_EXPECT_TRUE(compare(-INFINITY, -INFINITY));
  DTEST_EXPECT_FALSE(compare(INFINITY, -INFINITY));

  DTEST_INFO("NAN handling");
  DTEST_EXPECT_FALSE(compare(.1, NAN));
  DTEST_EXPECT_FALSE(compare(INFINITY, NAN));
  DTEST_EXPECT_FALSE(compare(-INFINITY, NAN));
  DTEST_EXPECT_FALSE(compare(NAN, NAN));

  DTEST_UNIT_END;
}
