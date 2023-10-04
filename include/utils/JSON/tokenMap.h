#ifndef TOKEN_MAP_H
#define TOKEN_MAP_H

#include "utils/JSON/token.h"

struct TokenMap;

struct TokenMap *createTokenMap();
unsigned int getTokenMapSize(struct TokenMap *map);
void expandTokenMap(struct TokenMap *map, char *key, struct Token *token);
char *getTokenMapKey(struct TokenMap *map, unsigned int n);
struct Token *getTokenMapElement(struct TokenMap *map, unsigned int n);
struct Token *readTokenMap(struct TokenMap *map, char *key);
char *tokenMapToString(struct TokenMap *map, int keys);
void destroyTokenMap(struct TokenMap *map);

#endif