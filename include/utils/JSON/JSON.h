/*
 * JSON data type implementation
 */

#ifndef JSON_H
#define JSON_H

struct JSON;

struct JSON *create_JSON();
struct JSON *file_to_JSON(char *filename);
char *JSON_to_string(struct JSON *json);
void destroy_JSON(struct JSON *json);

#endif /* JSON_H */
