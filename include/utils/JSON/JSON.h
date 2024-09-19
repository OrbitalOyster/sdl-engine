/*
 * JSON data type implementation
 */

#ifndef JSON_H
#define JSON_H

#include <stddef.h>

#include "utils/JSON/token.h"

#define MAX_JSON_KEY_LENGTH (63 + 1)

struct JSON;

struct JSON *create_JSON();

// Error handling
// void set_JSON_err(struct JSON *json, char *err, ...);
// char *get_JSON_err(struct JSON *json);

// For parsing
// int get_JSON_char(struct JSON *json);
// char *get_JSON_number_str(struct JSON *json);

// Conversions
// struct JSON *file_to_JSON(char *filename);
struct Token *file_to_JSON(char *filename);
char *JSON_to_string(struct Token *json);
struct Token *string_to_JSON(char *s);
// struct JSON *string_to_JSON(char *s);

int get_JSON_foo_number(struct Token *json, char *key, ...);
char *get_JSON_foo_string(struct Token *json, char *key, ...);

// int JSON_object_has_prop(struct JSON *json, char *prop);
int JSON_object_has_prop(struct Token *json, char *prop);

// size_t JSON_get_array_size(struct JSON *json, char *key, ...);
size_t JSON_get_array_size(struct Token *json, char *key, ...);

// Destructor
void destroy_JSON(struct JSON *json);

#endif /* JSON_H */
