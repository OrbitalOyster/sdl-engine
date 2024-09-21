#ifndef FOO_PARSER_H
#define FOO_PARSER_H

#include "token.h"

struct JSON_Parser;

struct JSON_Parser *create_JSON_parser();
struct Token *parse_JSON_file(struct JSON_Parser *parser, char *filename);
struct Token *parse_JSON_string(struct JSON_Parser *parser, char *s);
char *get_JSON_parser_err(struct JSON_Parser *parser);
void destroy_JSON_parser(struct JSON_Parser *parser);

#endif /* FOO_PARSER_H */
