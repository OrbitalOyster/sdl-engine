#include "utils/JSON/tokenMap.h"

#include <stdlib.h>
#include <string.h>

#include "utils/wtree.h"

struct TokenMap {
  unsigned int size;
  struct Token **content;
  struct WTree *tree;
};

struct TokenMap *createTokenMap() {
  struct TokenMap *result = calloc(1, sizeof(struct TokenMap));
  *result =
      (struct TokenMap){.size = 0, .content = NULL, .tree = createWTree()};
  return result;
}

unsigned int getTokenMapSize(struct TokenMap *map) {
  return map->size;
}

void expandTokenMap(struct TokenMap *map, char *key, struct Token *token) {
  map->size++;
  map->content = realloc(map->content, map->size * sizeof(struct Token *));
  map->content[map->size - 1] = token;
  expandWTree(map->tree, key, map->content[map->size - 1]);
}

struct Token *readTokenMap(struct TokenMap *map, char *key) {
  struct Token *result = getWTreeEndpoint(map->tree, key);
  if (result == NULL)
    result = createToken(Undefined, (union TokenValue){.empty = NULL});
  return result;
}

struct Token *getTokenMapElement(struct TokenMap *map, unsigned int n) {
  return map->content[n];
}

char *tokenMapToString(struct TokenMap *map, int keys) {
  size_t size = 1;
  char *result = calloc(size, sizeof(char));
  for (unsigned int i = 0; i < map->tree->size; i++) {
    char *key = map->tree->words[i];
    char *value = tokenToString(map->content[i]);
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

void destroyTokenMap(struct TokenMap *map) {
  destroyTree(map->tree);
  for (unsigned int i = 0; i < map->size; i++)
    destroyToken(map->content[i]);
  free(map->content);
  free(map);
}

