#ifndef TOKEN_MAP_H
#define TOKEN_MAP_H

#include "token.h"

struct TokenMap;

struct TokenMap *createTokenMap();
unsigned int getTokenMapSize(struct TokenMap *map);
void expandTokenMap(struct TokenMap *map, char *key, struct Token *token);
void expandTokenMapN(struct TokenMap *map, struct Token *token);
char **getTokenMapKeys(struct TokenMap *map);
struct Token *getTokenMapElement(struct TokenMap *map, unsigned int n);
struct Token *readTokenMap(struct TokenMap *map, char *key);
char *tokenMapToString(struct TokenMap *map, int keys);
void destroyTokenMap(struct TokenMap *map);

#endif /* TOKEN_MAP_H */
