#include "utils/JSON/JSON.h"

#include <stdlib.h>

#include "utils/debug.h"
#include "utils/JSON/parser.h"
#include "utils/JSON/stringify.h"

struct JSON {
  struct Token *root;
};

struct JSON *create_JSON() {
  struct JSON *json = calloc(1, sizeof(struct JSON));
  *json = (struct JSON) {.root = NULL};
  return json;
}

void parse_JSON_file(struct JSON *json, char *filename) {
  json->root = read_json_file(filename);
}

char *JSON_to_string(struct JSON *json) {
  return token_to_string(json->root);
}
