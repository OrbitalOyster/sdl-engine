#include "utils/JSON/token.h"

#include <stdlib.h>

#include "utils/debug.h"
#include "utils/JSON/tokenMap.h"

struct Token {
  enum TokenType type;
  union TokenValue value;
};

struct Token *createToken(enum TokenType type, union TokenValue value) {
  struct Token *result = calloc(1, sizeof(struct Token));
  *result = (struct Token){.type = type, .value = value};
  return result;
}

enum TokenType getTokenType(struct Token *token) {
  return token->type;
}

union TokenValue getTokenValue(struct Token *token) {
  return token->value;
}

struct Token *createObjectToken() {
  struct TokenMap *map = createTokenMap();
  struct Token *result = createToken(Object, (union TokenValue){.map = map});
  return result;
}

struct Token *createArrayToken() {
  struct TokenMap *map = createTokenMap();
  struct Token *result = createToken(Array, (union TokenValue){.map = map});
  return result;
}

struct Token *createNumberToken(int n) {
  struct Token *result = createToken(Number, (union TokenValue){.number = n});
  return result;
}

struct Token *createStringToken(char *s) {
  struct Token *result = createToken(String, (union TokenValue){.string = s});
  return result;
}

struct Token *createBooleanToken(int b) {
  struct Token *result =
      createToken(Boolean, (union TokenValue){.boolean = !!b});
  return result;
}

unsigned int getArrayTokenSize(struct Token *arr) {
  if (arr->type != Array)
    ERR(1, "Token is not array");
  return getTokenMapSize(arr->value.map);
}

struct Token *getArrayTokenElement(struct Token *arr, unsigned int n) {
  if (arr->type != Array)
    ERR(1, "Token is not array");
  return getTokenMapElement(arr->value.map, n);
}

void expandObjectToken(struct Token *obj, char *key, struct Token *token) {
  if (obj->type != Object)
    ERR(1, "Unable to expand token");
  expandTokenMap(obj->value.map, key, token);
}

void expandArrayToken(struct Token *arr, struct Token *token) {
  if (arr->type != Array)
    ERR(1, "Unable to expand token");
  unsigned int nextIndex = getArrayTokenSize(arr);
  char *key = calloc(MAX_STRING_LENGTH, sizeof(char));
  snprintf(key, MAX_STRING_LENGTH, "%i", nextIndex);
  expandTokenMap(arr->value.map, key, token);
  free(key);
}

void destroyToken(struct Token *token) {
  if (token->type == Object || token->type == Array)
    destroyTokenMap(token->value.map);
  free(token);
}

