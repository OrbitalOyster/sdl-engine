#ifndef PARSER_H
#define PARSER_H

#include "utils/JSON/token.h"
#include "utils/JSON/tokenMap.h"

struct Token *readFile(char *filename);

#endif
