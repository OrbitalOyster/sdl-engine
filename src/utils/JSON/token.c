#include "utils/JSON/token.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"

#include "utils/debug.h"

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
