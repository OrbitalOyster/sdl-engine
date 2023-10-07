#include "utils/JSON/stringify.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/tokenMap.h"

char *tokenMapToString(struct TokenMap *map, int keys);

char *objectTokenToString(struct TokenMap *map) {
  char *s = tokenMapToString(map, 1);
  char *result = calloc(2 + strlen(s) + 1, sizeof(char));
  strcat(result, "{");
  strcat(result, s);
  strcat(result, "}");
  free(s);
  return result;
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

char *tokenToString(struct Token *token) {
  char *result = NULL;
  union TokenValue value = getTokenValue(token);
  switch (getTokenType(token)) {
  case Undefined:
    break;
  case Object:
    result = objectTokenToString(value.map);
    break;
  case Array:
    result = arrayTokenToString(value.map);
    break;
  case Number:
    result = calloc(MAX_NUMBER_LENGTH, sizeof(char));
    snprintf(result, MAX_NUMBER_LENGTH, "%i", value.number);
    break;
  case String:
    result = calloc(MAX_STRING_LENGTH, sizeof(char));
    snprintf(result, MAX_STRING_LENGTH, "\"%s\"", value.string);
    break;
  case Boolean:
    result = calloc(6, sizeof(char));
    if (value.boolean)
      snprintf(result, 5, "%s", "true");
    else
      snprintf(result, 6, "%s", "false");
    break;
  case Null:
    result = calloc(5, sizeof(char));
    snprintf(result, 5, "%s", "null");
    break;
  }
  return result;
}

char *tokenMapToString(struct TokenMap *map, int keys) {
  size_t size = 1;
  char *result = calloc(size, sizeof(char));
  for (unsigned int i = 0; i < getTokenMapSize(map); i++) {
    char *key = getTokenMapKey(map, i);
    char *value = tokenToString(getTokenMapElement(map, i));
    if (i) { // ", "
      size += (2 + strlen(value)) * sizeof(char);
      result = realloc(result, size);
      strcat(result, ", ");
    }
    if (keys) {
      size += (strlen(key) + 2) * sizeof(char);
      result = realloc(result, size);
      strcat(result, key);
      strcat(result, ": ");
    }
    size += strlen(value) * sizeof(char);
    result = realloc(result, size);
    strcat(result, value);
    free(value);
  }
  return result;
}

