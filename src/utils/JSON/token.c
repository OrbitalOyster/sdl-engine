#include "utils/JSON/token.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"

#include "utils/debug.h"

const char *TOKEN_DELIMITERS = "/";

struct Token {
  enum TokenType type;
  union TokenValue value;
};

const char *token_types_str[] = {"Undefined", "Object",  "Array", "Number",
                                 "String",    "Boolean", "Null"};

struct Token *create_token(enum TokenType type, union TokenValue value) {
  struct Token *result = calloc(1, sizeof(struct Token));
  if (!result)
    ERR(1, "Out of memory");
  *result = (struct Token){.type = type, .value = value};
  return result;
}

enum TokenType get_token_type(struct Token *token) { return token->type; }

const char *token_type_to_string(int t) { return token_types_str[t]; }

union TokenValue get_token_value(struct Token *token) { return token->value; }


static struct Token *get_JSON_foo(struct Token *token, enum TokenType token_type,
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
  struct Token *current_token = token;
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
        // set_JSON_err(json, "Undefined prop: %s", next_key);
        ERRF(1, "Undefined prop: %s", next_key);
        return NULL;
      }
      break;
    case Array: {
      size_t n = (size_t)atoi(next_key);
      if (n >= get_token_array_size(value.array)) {
        // set_JSON_err(json, "Array prop out of bounds (%i)", n);
        ERRF(1, "Array prop out of bounds (%lu)", n);
        return NULL;
      }
      current_token = get_token_array_element(value.array, n);
      break;
    }
    default: {
      // set_JSON_err(json, "Array prop out of bounds");
      ERRF(1, "Array prop out of bounds (%s)", next_key);
      return NULL;
    }
    }
  }
  free(key_s);

  // Check if types match
  enum TokenType result_type = get_token_type(current_token);
  if (result_type != token_type) {
    /*
    set_JSON_err(json, "Type mismatch for token \"%s\" (expected %s, got %s)",
                 key, token_type_to_string(token_type),
                 token_type_to_string(result_type));
                 */
    ERRF(1, "Type mismatch for token \"%s\" (expected %s, got %s)",
                 key, token_type_to_string(token_type),
                 token_type_to_string(result_type));
    return NULL;
  }

  return current_token;
}

int get_JSON_foo_number(struct Token *token, char *key, ...) {
  va_list args;
  va_start(args, key);
  struct Token *number_token = get_JSON_foo(token, Number, key, args);
//  if (json->err)
//    return 0;
  va_end(args);
  return get_token_value(number_token).number;
}

char *get_JSON_foo_string(struct Token *token, char *key, ...) {
  va_list args;
  va_start(args, key);
  struct Token *string_token = get_JSON_foo(token, String, key, args);
//  if (json->err)
//    return "";
  va_end(args);
  return get_token_value(string_token).string;
}

size_t JSON_get_array_size(struct Token *token, char *key, ...) {
  va_list args;
  va_start(args, key);
  struct Token *array_token = get_JSON_foo(token, Array, key, args);
//  if (json->err)
//    return 0;
  va_end(args);

  union TokenValue value = get_token_value(array_token);
  return get_token_array_size(value.array);
}

// Destructor
void destroy_token(struct Token *token) {
  if (token == NULL)
    ERR(1, "Attempt to destroy NULL token");
  switch (token->type) {
  case Object:
    destroy_token_map(token->value.map);
    break;
  case Array:
    destroy_token_array(token->value.array);
    break;
  case String:
    free(token->value.string);
    break;
  case Undefined:
  case Number:
  case Boolean:
  case Null:
    break;
  }
  free(token);
}
