#include "utils/JSON/token-map.h"

#include <stdlib.h>
#include <string.h>

#include "utils/wtree.h"

struct TokenMap {
  size_t size;
  struct Token **content;
  struct WTree *tree;
};

struct TokenMap *create_token_map() {
  struct TokenMap *result = calloc(1, sizeof(struct TokenMap));
  *result =
      (struct TokenMap){.size = 0, .content = NULL, .tree = create_wtree()};
  return result;
}

size_t get_token_map_size(struct TokenMap *map) { return map->size; }

void expand_token_map(struct TokenMap *map, char *key, struct Token *token) {
  map->size++;
  map->content = realloc(map->content, map->size * sizeof(struct Token *));
  map->content[map->size - 1] = token;
  expand_wtree(map->tree, key, map->content[map->size - 1]);
}

char **get_token_map_keys(struct TokenMap *map) {
  return get_wtree_words(map->tree);
}

// Helper functions
int check_token_map_has_key(struct TokenMap *map, char *key) {
  return check_wtree_has_word(map->tree, key);
}

struct Token *get_token_map_element(struct TokenMap *map, char *key) {
  return get_wtree_endpoint(map->tree, key);
}

void destroy_token_map(struct TokenMap *map) {
  destroy_wtree(map->tree);
  for (size_t i = 0; i < map->size; i++)
    destroy_token(map->content[i]);
  free(map->content);
  free(map);
}
