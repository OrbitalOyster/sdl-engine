#include "utils/JSON/JSON.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/parser.h"
#include "utils/JSON/stringify.h"
#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"
#include "utils/JSON/token.h"

#include "utils/debug.h"

// #define MAX_JSON_ERR_LENGTH (255 + 1)

// JSON tokens delimeters

/*
void set_JSON_err(struct JSON *json, char *err, ...) {
  // Already having issues
  if (json->err)
    free(json->err);
  // Magic
  char *err_s = calloc(MAX_JSON_ERR_LENGTH, sizeof(char));
  va_list args;
  va_start(args, err);
  int chars = vsnprintf(err_s, MAX_JSON_ERR_LENGTH, err, args);
  if (chars >= MAX_JSON_ERR_LENGTH)
    WARNF("JSON error too large (%i)", chars);
  va_end(args);
  // Copy to JSON err
  json->err = calloc(strlen(err_s) + 1, sizeof(char));
  strcpy(json->err, err_s);
}
*/

// char *get_JSON_err(struct JSON *json) { return json->err; }

/*
struct Token *file_to_JSON(char *filename) {
  return parse_JSON_file(filename);
}

struct Token *string_to_JSON(char *s) {
  return parse_JSON_string(s);
}
*/

// char *JSON_to_string(struct Token *json) { return token_to_string(json); }

/*
int JSON_object_has_prop(struct Token *json, char *prop) {
  union TokenValue value = get_token_value(json);
  struct TokenMap *map = value.map;
  return check_token_map_has_key(map, prop);
}
*/