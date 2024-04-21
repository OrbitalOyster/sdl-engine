#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "token-map.h"

struct Token *read_json_file(char *filename);
struct Token *read_json_string(char *str);

#endif /* PARSER_H */
