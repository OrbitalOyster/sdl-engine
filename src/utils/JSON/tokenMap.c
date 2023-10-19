#include "utils/JSON/tokenMap.h"

#include <stdio.h>
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

unsigned int getTokenMapSize(struct TokenMap *map) { return map->size; }

void expandTokenMap(struct TokenMap *map, char *key, struct Token *token) {
  map->size++;
  map->content = realloc(map->content, map->size * sizeof(struct Token *));
  map->content[map->size - 1] = token;
  expandWTree(map->tree, key, map->content[map->size - 1]);
}

void expandTokenMapN(struct TokenMap *map, struct Token *token) {
  char *key = calloc(MAX_KEY_LENGTH, sizeof(char));
  snprintf(key, MAX_KEY_LENGTH, "%i", map->size);
  expandTokenMap(map, key, token);
  free(key);
}

char *getTokenMapKey(struct TokenMap *map, unsigned int n) {
  return map->tree->words[n];
}

struct Token *getTokenMapElement(struct TokenMap *map, unsigned int n) {
  return map->content[n];
}

struct Token *readTokenMap(struct TokenMap *map, char *key) {
  struct Token *result = getWTreeEndpoint(map->tree, key);
  if (result == NULL)
    result = createUndefinedToken();
  return result;
}

void destroyTokenMap(struct TokenMap *map) {
  destroyWTree(map->tree);
  for (unsigned int i = 0; i < map->size; i++)
    destroyToken(map->content[i]);
  free(map->content);
  free(map);
}
