#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#include "JSON.h"

void skip_whitespaces(struct JSON *json,
                      int (*get_next_char)(struct JSON *json),
                      void (*rewind)(struct JSON *json));
struct Token *parse_next_token(struct JSON *json,
                               int (*get_next_char)(struct JSON *json),
                               void (*rewind)(struct JSON *json));
struct Token *read_json_file(char *filename, size_t *char_num, size_t *line_num,
                             size_t *col_num);
struct Token *read_json_string(char *str);

#endif /* PARSER_H */
