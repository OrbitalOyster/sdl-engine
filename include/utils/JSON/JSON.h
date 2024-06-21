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
char *get_JSON_number_str(struct JSON *json);
struct JSON *file_to_JSON(char *filename);
char *JSON_to_string(struct JSON *json);
struct JSON *string_to_JSON(char *s);

int JSON_is_undefined(struct JSON *json);
int JSON_is_object(struct JSON *json);
int JSON_is_array(struct JSON *json);
int JSON_is_number(struct JSON *json);
int JSON_is_string(struct JSON *json);
int JSON_is_boolean(struct JSON *json);
int JSON_is_null(struct JSON *json);

int JSON_object_has_prop(struct JSON *json, char *prop);

int JSON_get_number_prop(struct JSON *json, char *prop);
// int JSON_get_number_prop(struct JSON *json, char *prop);
char *JSON_get_string_prop(struct JSON *json, char *prop);

void destroy_JSON(struct JSON *json);

#endif /* JSON_H */
