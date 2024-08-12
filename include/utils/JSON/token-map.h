#ifndef TOKEN_MAP_H
#define TOKEN_MAP_H

#include <stddef.h>

#include "token.h"

struct TokenMap;

struct TokenMap *create_token_map();
size_t get_token_map_size(struct TokenMap *map);
void expand_token_map(struct TokenMap *map, char *key, struct Token *token);
char **get_token_map_keys(struct TokenMap *map);
int check_token_map_has_key(struct TokenMap *map, char *key);
struct Token *get_token_map_element(struct TokenMap *map, char *key);
void destroy_token_map(struct TokenMap *map);

#endif /* TOKEN_MAP_H */
