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

#define MAX_JSON_KEY_LENGTH (63 + 1)
#define MAX_JSON_ERR_LENGTH (255 + 1)

// JSON tokens delimeters
const char *TOKEN_DELIMITERS = "/";

struct JSON {
  struct Token *root;
  char *err;
};

struct JSON *create_JSON() {
  struct JSON *json = calloc(1, sizeof(struct JSON));
  *json = (struct JSON){.root = NULL, .err = NULL};
  return json;
}

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

char *get_JSON_err(struct JSON *json) { return json->err; }

struct JSON *file_to_JSON(char *filename) {
  struct JSON *json = create_JSON();
  json->root = parse_JSON_file(filename);
  return json;
}

struct JSON *string_to_JSON(char *s) {
  struct JSON *json = create_JSON();
  json->root = parse_JSON_string(s);
  return json;
}

char *JSON_to_string(struct JSON *json) { return token_to_string(json->root); }

static struct Token *get_JSON_foo(struct JSON *json, enum TokenType token_type,
                                  char *key, va_list args) {
  // Magic
  char *key_s = calloc(MAX_JSON_KEY_LENGTH, sizeof(char));
  int chars = vsnprintf(key_s, MAX_JSON_KEY_LENGTH, key, args);
  if (chars >= MAX_JSON_KEY_LENGTH) {
    WARNF("JSON key too large (%i)", chars);
    return NULL;
  }

  INFO2F("Reading deep token: %s", key_s);
  char *next_key = NULL;
  struct Token *current_token = json->root;
  while (1) {
    if (!next_key) // First run
      next_key = strtok(key_s, TOKEN_DELIMITERS);
    else
      next_key = strtok(NULL, TOKEN_DELIMITERS);
    INFO2F("Next key: %s", next_key);
    if (next_key == NULL)
      break;
    enum TokenType type = get_token_type(current_token);
    union TokenValue value = get_token_value(current_token);
    switch (type) {
    case Object:
      if (check_token_map_has_key(value.map, next_key)) {
        current_token = get_token_map_element(value.map, next_key);
      } else {
        set_JSON_err(json, "Undefined prop: %s", next_key);
        return NULL;
      }
      break;
    case Array: {
      size_t n = (size_t)atoi(next_key);
      if (n >= get_token_array_size(value.array)) {
        set_JSON_err(json, "Array prop out of bounds (%i)", n);
        return NULL;
      }
      current_token = get_token_array_element(value.array, n);
      break;
    }
    default: {
      set_JSON_err(json, "Array prop out of bounds");
      return NULL;
    }
    }
  }
  free(key_s);

  // Check if types match
  enum TokenType result_type = get_token_type(current_token);
  if (result_type != token_type) {
    set_JSON_err(json, "Type mismatch for token \"%s\" (expected %s, got %s)",
                 key, token_type_to_string(token_type),
                 token_type_to_string(result_type));
    return NULL;
  }

  return current_token;
}

int get_JSON_foo_number(struct JSON *json, char *key, ...) {
  va_list args;
  va_start(args, key);
  struct Token *token = get_JSON_foo(json, Number, key, args);
  if (json->err)
    return 0;
  va_end(args);
  return get_token_value(token).number;
}

char *get_JSON_foo_string(struct JSON *json, char *key, ...) {
  va_list args;
  va_start(args, key);
  struct Token *token = get_JSON_foo(json, String, key, args);
  if (json->err)
    return "";
  va_end(args);
  return get_token_value(token).string;
}

int JSON_object_has_prop(struct JSON *json, char *prop) {
  union TokenValue value = get_token_value(json->root);
  struct TokenMap *map = value.map;
  return check_token_map_has_key(map, prop);
}

size_t JSON_get_array_size(struct JSON *json, char *key, ...) {
  va_list args;
  va_start(args, key);
  struct Token *token = get_JSON_foo(json, Array, key, args);
  if (json->err)
    return 0;
  va_end(args);

  union TokenValue value = get_token_value(token);
  return get_token_array_size(value.array);
}

// Destructor
void destroy_JSON(struct JSON *json) {
  if (json == NULL)
    ERR(1, "Attempt to destroy NULL JSON");
  destroy_token(json->root);
  free(json);
}
