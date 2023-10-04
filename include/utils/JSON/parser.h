#ifndef PARSER_H
#define PARSER_H

#include "utils/JSON/token.h"

void readFile(char *filename);
struct Token *stringToToken(char *s);

#endif
