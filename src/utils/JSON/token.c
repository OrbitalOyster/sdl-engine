#include "utils/JSON/token.h"

#include <stddef.h>
#include <stdlib.h>

#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"

#include "utils/debug.h"

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

enum TokenType get_token_type(struct Token *token) { return token->type; }

union TokenValue get_token_value(struct Token *token) { return token->value; }

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
