/*
 * JSON data type implementation
 */

#ifndef JSON_H
#define JSON_H

struct JSON;

struct JSON *create_JSON();
void set_JSON_err(struct JSON *json, char *err);
char *get_JSON_err(struct JSON *json);

int get_JSON_char(struct JSON *json);

// void skip_next_JSON_char(struct JSON *json);

struct JSON *file_to_JSON(char *filename);
char *JSON_to_string(struct JSON *json);

struct JSON *string_to_JSON(char *s);

void destroy_JSON(struct JSON *json);

#endif /* JSON_H */
