/*
 * JSON data type implementation
 */

#ifndef JSON_H
#define JSON_H

#include "utils/JSON/token.h"

struct JSON;

struct JSON *create_JSON();
void parse_JSON_file(struct JSON *json, char *filename);
char *JSON_to_string(struct JSON *json);
struct JSON *string_to_JSON(char *s);
void destroy_JSON(struct JSON *json);

#endif /* JSON_H */
