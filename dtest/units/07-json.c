// obj/utils/wtree.o obj/utils/dstrings.o obj/utils/JSON/*.o obj/utils/qsort.o
// 0

#include <stdlib.h>

#include "JSON/token.h"
#include "JSON/parser.h"
#include "JSON/stringify.h"

#include <utils/debug.h>

int main() {
  DTEST_UNIT_START("JSON parser");

  struct JSON_Parser *parser1 = create_JSON_parser();
  struct Token *json1 = parse_JSON_file(parser1, "assets/sample.json");
  char *s1 = token_to_string(json1);

  struct JSON_Parser *parser2 = create_JSON_parser();
  struct Token *json2 = parse_JSON_string(parser2, s1);
  char *s2 = token_to_string(json2);

  // strcmp returns 0 on equal strings
  DTEST_EXPECT_FALSE(strcmp(s1, s2));

  destroy_JSON_parser(parser1);
  destroy_JSON_parser(parser2);

  destroy_token(json1);
  free(s1);
  destroy_token(json2);
  free(s2);

  DTEST_UNIT_END;
}
