#include "utils/JSON/token.h"

#include <stdlib.h>

#include "utils/JSON/tokenMap.h"
#include "utils/debug.h"

struct Token {
  enum TokenType type;
  union TokenValue value;
};

struct Token *create_token(enum TokenType type, union TokenValue value) {
  struct Token *result = calloc(1, sizeof(struct Token));
  *result = (struct Token){.type = type, .value = value};
  return result;
}

enum TokenType get_token_type(struct Token *token) { return token->type; }

union TokenValue get_token_value(struct Token *token) { return token->value; }

struct Token *create_undefined_token() {
  struct Token *result = calloc(1, sizeof(struct Token));
  *result = (struct Token){.type = Undefined};
  return result;
}

struct Token *create_object_token() {
  struct TokenMap *map = create_token_map();
  struct Token *result = create_token(Object, (union TokenValue){.map = map});
  return result;
}

struct Token *create_array_token() {
  struct TokenMap *map = create_token_map();
  struct Token *result = create_token(Array, (union TokenValue){.map = map});
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
  *result = (struct Token){.type = Null};
  return result;
}

unsigned int get_array_token_size(struct Token *arr) {
  if (arr->type != Array)
    ERR(1, "Token is not array");
  return get_token_map_size(arr->value.map);
}

struct Token *get_array_token_element(struct Token *arr, unsigned int n) {
  if (arr->type != Array)
    ERR(1, "Token is not array");
  return get_token_map_element(arr->value.map, n);
}

void expand_object_token(struct Token *obj, char *key, struct Token *token) {
  if (obj->type != Object)
    ERR(1, "Unable to expand token");
  expand_token_map(obj->value.map, key, token);
}

void expand_array_token(struct Token *arr, struct Token *token) {
  if (arr->type != Array)
    ERR(1, "Unable to expand token");
  unsigned int next_index = get_array_token_size(arr);
  char *key = calloc(MAX_STRING_LENGTH, sizeof(char));
  snprintf(key, MAX_STRING_LENGTH, "%i", next_index);
  expand_token_map(arr->value.map, key, token);
  free(key);
}

void destroy_token(struct Token *token) {
  if (token->type == Object || token->type == Array)
    destroy_token_map(token->value.map);
  if (token->type == String)
    free(token->value.string);
  free(token);
}
