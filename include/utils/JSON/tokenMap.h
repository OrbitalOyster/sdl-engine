#ifndef TOKEN_MAP_H
#define TOKEN_MAP_H

#include "token.h"

struct TokenMap;

struct TokenMap *create_token_map();
unsigned int get_token_map_size(struct TokenMap *map);
void expand_token_map(struct TokenMap *map, char *key, struct Token *token);
void expandTokenMapN(struct TokenMap *map, struct Token *token);
char **get_token_map_keys(struct TokenMap *map);
struct Token *get_token_map_element(struct TokenMap *map, unsigned int n);
struct Token *read_token_map(struct TokenMap *map, char *key);
void destroy_token_map(struct TokenMap *map);

#endif /* TOKEN_MAP_H */
