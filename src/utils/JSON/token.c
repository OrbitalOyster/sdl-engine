#include "utils/JSON/token.h"

#include <stdlib.h>
#include <string.h>

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

char *arrayTokenToString(struct TokenMap *map) {
  char *s = tokenMapToString(map, 0);
  char *result = calloc(2 + strlen(s) + 1, sizeof(char));
  strcat(result, "[");
  strcat(result, s);
  strcat(result, "]");
  free(s);
  return result;
}

char *objectTokenToString(struct TokenMap *map) {
  char *s = tokenMapToString(map, 1);
  char *result = calloc(2 + strlen(s) + 1, sizeof(char));
  strcat(result, "{");
  strcat(result, s);
  strcat(result, "}");
  free(s);
  return result;
}

char *tokenToString(struct Token *token) {
  char *result = NULL;
  switch (token->type) {
  case Undefined:
    INFO("Undefined");
    break;
  case Object:
    result = objectTokenToString(token->value.map);
    break;
  case Array:
    result = arrayTokenToString(token->value.map);
    break;
  case Number:
    result = calloc(MAX_NUMBER_LENGTH, sizeof(char));
    snprintf(result, MAX_NUMBER_LENGTH, "%i", token->value.number);
    break;
  case String:
    result = calloc(MAX_STRING_LENGTH, sizeof(char));
    snprintf(result, MAX_STRING_LENGTH, "\"%s\"", token->value.string);
    break;
  case Boolean:
    result = calloc(6, sizeof(char));
    if (token->value.boolean)
      snprintf(result, 5, "%s", "true");
    else
      snprintf(result, 6, "%s", "false");
    break;
  case Eof:
    break;
  }
  return result;
}

void destroyToken(struct Token *token) {
  if (token->type == Object || token->type == Array)
    destroyTokenMap(token->value.map);
  free(token);
}

