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

struct TokenMap *create_token_map() {
  struct TokenMap *result = calloc(1, sizeof(struct TokenMap));
  *result =
      (struct TokenMap){.size = 0, .content = NULL, .tree = create_wtree()};
  return result;
}

unsigned int get_token_map_size(struct TokenMap *map) { return map->size; }

void expand_token_map(struct TokenMap *map, char *key, struct Token *token) {
  map->size++;
  map->content = realloc(map->content, map->size * sizeof(struct Token *));
  map->content[map->size - 1] = token;
  expand_wtree(map->tree, key, map->content[map->size - 1]);
}

void expandTokenMapN(struct TokenMap *map, struct Token *token) {
  char *key = calloc(MAX_KEY_LENGTH, sizeof(char));
  snprintf(key, MAX_KEY_LENGTH, "%i", map->size);
  expand_token_map(map, key, token);
  free(key);
}

char **get_token_map_keys(struct TokenMap *map) {
  return get_wtree_words(map->tree);
}

struct Token *get_token_map_element(struct TokenMap *map, unsigned int n) {
  return map->content[n];
}

struct Token *read_token_map(struct TokenMap *map, char *key) {
  struct Token *result = get_wtree_endpoint(map->tree, key);
  if (result == NULL)
    result = create_undefined_token();
  return result;
}

void destroy_token_map(struct TokenMap *map) {
  destroy_wtree(map->tree);
  for (unsigned int i = 0; i < map->size; i++)
    destroy_token(map->content[i]);
  free(map->content);
  free(map);
}
