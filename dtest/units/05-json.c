// obj/utils/wtree.o obj/utils/JSON/*.o
// 0

#include <utils/debug.h>

#include "utils/JSON/token.h"
#include "utils/JSON/parser.h"

int main() {
  DTEST_UNIT_START("JSON parser");
  struct Token *token = readFile("s.json");
  char *s = tokenToString(token);
  INFOF("Token to string: %s", s);
  DTEST_UNIT_END
  return 0;
}
