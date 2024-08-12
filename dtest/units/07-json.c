// obj/utils/wtree.o obj/utils/dstrings.o obj/utils/JSON/*.o obj/utils/qsort.o
// 0

#include <stdlib.h>

#include <utils/debug.h>

#include "utils/JSON/token.h"
#include "utils/JSON/parser.h"
#include "utils/JSON/stringify.h"

int main() {
  DTEST_UNIT_START("JSON parser");
  struct JSON *json1 = file_to_JSON("assets/sample.json");
  char *s1 = JSON_to_string(json1);
  struct JSON *json2 = string_to_JSON(s1);
  char *s2 = JSON_to_string(json2);

  // strcmp returns 0 on equal strings
  DTEST_EXPECT_FALSE(strcmp(s1, s2));

  destroy_JSON(json1);
  free(s1);
  destroy_JSON(json2);
  free(s2);

  DTEST_UNIT_END;
}
