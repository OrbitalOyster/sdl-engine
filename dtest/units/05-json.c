// obj/utils/wtree.o obj/utils/jsonParser.o obj/utils/JSON/*.o
// 0

#include "utils/jsonParser.h"

int main() {
  DTEST_UNIT_START("JSON parser");
  // readFile("s.json");
  tokenTest();
  DTEST_UNIT_END
  return 0;
}
