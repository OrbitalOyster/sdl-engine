#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "tokenMap.h"

struct Token *read_json_file(char *filename);

#endif /* PARSER_H */
