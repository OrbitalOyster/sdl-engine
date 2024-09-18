#ifndef FOO_PARSER_H
#define FOO_PARSER_H

#include "token.h"

struct Token *parse_JSON_file(char *filename);
struct Token *parse_JSON_string(char *s);

#endif /* FOO_PARSER_H */
