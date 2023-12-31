// obj/utils/dstrings.o
// 0

#include "utils/dstrings.h"

#include <stdlib.h>

int main() {
  DTEST_UNIT_START("DStrings");
  DTEST_EXPECT_STRING(uint_to_binary(0), "0");
  DTEST_EXPECT_STRING(uint_to_binary(1), "1");
  DTEST_EXPECT_STRING(uint_to_binary(3), "11");
  DTEST_EXPECT_STRING(uint_to_binary(255), "11111111");
  DTEST_EXPECT_STRING(uint_to_binary(4000000000),
                      "11101110011010110010100000000000");
  DTEST_UNIT_END;
}
