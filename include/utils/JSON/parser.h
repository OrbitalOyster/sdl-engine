#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#include "JSON.h"
#include "token-map.h"
#include "token.h"

void skip_whitespaces(void *json, int (*get_next_char)(void *json),
                      void (*rewind)(void *json));

struct Token *parse_next_token(void *json, int (*get_next_char)(void *json),
                          void (*rewind)(void *json));

struct Token *read_json_file(char *filename, size_t *char_num, size_t *line_num,
                             size_t *col_num);
struct Token *read_json_string(char *str);

#endif /* PARSER_H */
