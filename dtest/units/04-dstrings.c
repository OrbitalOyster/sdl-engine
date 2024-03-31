// obj/utils/dstrings.o
// 0

#include "utils/dstrings.h"

#include <stdlib.h>

int main() {
  DTEST_UNIT_START("DStrings");

  char *str_0 = uint_to_binary(0);
  DTEST_EXPECT_STRING(str_0, "0");
  free(str_0);

  char *str_1 = uint_to_binary(1);
  DTEST_EXPECT_STRING(str_1, "1");
  free(str_1);

  char *str_3 = uint_to_binary(3);
  DTEST_EXPECT_STRING(str_3, "11");
  free(str_3);

  char *str_255 = uint_to_binary(255);
  DTEST_EXPECT_STRING(str_255, "11111111");
  free(str_255);

  char *str_4billion = uint_to_binary(4000000000);
  DTEST_EXPECT_STRING(str_4billion, "11101110011010110010100000000000");
  free(str_4billion);

  DTEST_UNIT_END;
}
