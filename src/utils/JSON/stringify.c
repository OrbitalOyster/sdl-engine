#include "utils/JSON/stringify.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/tokenMap.h"

static char *token_map_to_string(struct TokenMap *map, int keys);

static char *object_token_to_string(struct TokenMap *map) {
  char *s = token_map_to_string(map, 1);
  char *result = calloc(2 + strlen(s) + 1, sizeof(char));
  strcat(result, "{");
  strcat(result, s);
  strcat(result, "}");
  free(s);
  return result;
}

static char *array_token_to_string(struct TokenMap *map) {
  char *s = token_map_to_string(map, 0);
  char *result = calloc(2 + strlen(s) + 1, sizeof(char));
  strcat(result, "[");
  strcat(result, s);
  strcat(result, "]");
  free(s);
  return result;
}

static char *token_map_to_string(struct TokenMap *map, int nkeys) {
  size_t size = 1;
  char *result = calloc(size, sizeof(char));
  char **keys = get_token_map_keys(map);
  for (unsigned int i = 0; i < get_token_map_size(map); i++) {
    char *key = keys[i];
    char *value = token_to_string(get_token_map_element(map, i));
    if (i) { // ", "
      size += (2 + strlen(value)) * sizeof(char);
      result = realloc(result, size);
      strcat(result, ", ");
    }
    if (nkeys) {
      size += (strlen(key) + 2) * sizeof(char);
      result = realloc(result, size);
      strcat(result, key);
      strcat(result, ": ");
    }
    size += strlen(value) * sizeof(char);
    result = realloc(result, size);
    strcat(result, value);
    free(value);
    free(key);
  }
  free(keys);
  return result;
}

char *token_to_string(struct Token *token) {
  char *result = NULL;
  union TokenValue value = get_token_value(token);
  switch (get_token_type(token)) {
  case Undefined:
    break;
  case Object:
    result = object_token_to_string(value.map);
    break;
  case Array:
    result = array_token_to_string(value.map);
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

