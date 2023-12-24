// obj/utils/wtree.o obj/utils/JSON/*.o obj/utils/qsort.o
// 0

#include <stdlib.h>

#include <utils/debug.h>

#include "utils/JSON/token.h"
#include "utils/JSON/parser.h"

int main() {
  DTEST_UNIT_START("JSON parser");
  struct Token *token = readFile("sample.json");
  char *s = tokenToString(token);
  INFOF("Token to string: %s", s);
  destroyToken(token);
  free(s);
  DTEST_UNIT_END;
}
