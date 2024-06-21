#include "utils/JSON/token.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"

#include "utils/debug.h"

// JSON tokens delimeters
const char *TOKEN_DELIMITERS = "/";

struct Token {
  enum TokenType type;
  union TokenValue value;
};

struct Token *create_token(enum TokenType type, union TokenValue value) {
  struct Token *result = calloc(1, sizeof(struct Token));
  if (!result)
    ERR(1, "Out of memory");
  *result = (struct Token){.type = type, .value = value};
  return result;
}

struct Token *get_token(struct Token *token, char *key) {
 // Make a copy of key
  size_t l = strlen(key) + 1;
  char *s = calloc(l, sizeof(char));
  memcpy(s, key, l);

  INFO2F("Reading deep token: %s", key);

  char *next_key = NULL;

  struct Token *current_token = token;

  while (1) {

    if (!next_key) // First run
      next_key = strtok(s, TOKEN_DELIMITERS);
    else
      next_key = strtok(NULL, TOKEN_DELIMITERS);

    INFO2F("Next key: %s", next_key);
    if (next_key == NULL)
      break;
    switch (current_token->type) {
      case Object:
        if (check_token_map_has_key(current_token->value.map, next_key)) {
          current_token = get_token_map_element(current_token->value.map, next_key);
        }
        else
          // TODO: Set JSON error
          ERRF(1, "Attempt to get undefined object prop (%s)", next_key);
        break;
      case Array: {
        size_t n = (size_t) atoi(next_key);
        // TODO: Set JSON error
        if (n >= get_token_array_size(current_token->value.array))
          ERRF(1, "Attempt to get out of bounds array element (%li)", n);
        current_token = get_token_array_element(current_token->value.array, n);
        break;
      }
      default:
        ERR(1, "Attempt to get deep value from non-map token");
    }
  }

  free(s);
  return current_token;
}

enum TokenType get_token_type(struct Token *token) { return token->type; }

union TokenValue get_token_value(struct Token *token) { return token->value; }

union TokenValue get_token_value_deep(struct Token *token, char *key) {
  // Make a copy of key
  size_t l = strlen(key) + 1;
  char *s = calloc(l, sizeof(char));
  memcpy(s, key, l);

  INFO2F("Reading deep token: %s", key);

  char *next_key = NULL;

  struct Token *current_token = token;

  while (1) {

    if (!next_key) // First run
      next_key = strtok(s, TOKEN_DELIMITERS);
    else
      next_key = strtok(NULL, TOKEN_DELIMITERS);

    INFO2F("Next key: %s", next_key);
    if (next_key == NULL)
      break;
    switch (current_token->type) {
      case Object:
        if (check_token_map_has_key(current_token->value.map, next_key)) {
          current_token = get_token_map_element(current_token->value.map, next_key);
        }
        else
          // TODO: Set JSON error
          ERRF(1, "Attempt to get undefined object prop (%s)", next_key);
        break;
      case Array: {
        size_t n = (size_t) atoi(next_key);
        // TODO: Set JSON error
        if (n >= get_token_array_size(current_token->value.array))
          ERRF(1, "Attempt to get out of bounds array element (%li)", n);
        current_token = get_token_array_element(current_token->value.array, n);
        break;
      }
      default:
        ERR(1, "Attempt to get deep value from non-map token");
    }
  }

  free(s);
  return current_token->value;
}

struct Token *create_undefined_token() {
  struct Token *result = calloc(1, sizeof(struct Token));
  if (!result)
    ERR(1, "Out of memory");
  *result = (struct Token){.type = Undefined};
  return result;
}

struct Token *create_object_token() {
  struct TokenMap *map = create_token_map();
  struct Token *result = create_token(Object, (union TokenValue){.map = map});
  return result;
}

struct Token *create_array_token() {
  struct TokenArray *array = create_token_array();
  struct Token *result =
      create_token(Array, (union TokenValue){.array = array});
  return result;
}

struct Token *create_number_token(int n) {
  struct Token *result = create_token(Number, (union TokenValue){.number = n});
  return result;
}

struct Token *create_string_token(char *s) {
  struct Token *result = create_token(String, (union TokenValue){.string = s});
  return result;
}

struct Token *create_boolean_token(int b) {
  struct Token *result =
      create_token(Boolean, (union TokenValue){.boolean = !!b});
  return result;
}

struct Token *create_null_token() {
  struct Token *result = calloc(1, sizeof(struct Token));
  if (!result)
    ERR(1, "Out of memory");
  *result = (struct Token){.type = Null};
  return result;
}

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
