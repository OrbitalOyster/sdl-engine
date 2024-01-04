// obj/utils/wtree.o obj/utils/dstrings.o obj/utils/JSON/*.o obj/utils/qsort.o
// 0

#include <stdlib.h>

#include <utils/debug.h>

#include "utils/JSON/token.h"
#include "utils/JSON/parser.h"
#include "utils/JSON/stringify.h"

int main() {
  DTEST_UNIT_START("JSON parser");
  struct Token *token = read_json_file("s.json");
  char *s = token_to_string(token);
  INFOF("Token to string: %s", s);
  destroy_token(token);
  free(s);
  DTEST_UNIT_END;
}
