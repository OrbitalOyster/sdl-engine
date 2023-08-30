// obj/utils/jsonParser.o
// 0

#include "utils/jsonParser.h"

int main() {
  DTEST_UNIT_START("JSON parser");
  readFile("s.json");
  DTEST_UNIT_END
  return 0;
}
