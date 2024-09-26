#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#include "JSON/JSON.h"

struct Token *create_token(enum TokenType type, union TokenValue value);
enum TokenType get_token_type(struct Token *token);
const char *token_type_to_string(enum TokenType t);
union TokenValue get_token_value(struct Token *token);
char *get_token_err(struct Token *token);
int read_token_number(struct Token *token, char *key, ...);
char *read_token_string(struct Token *token, char *key, ...);
struct Token *read_token_token(struct Token *token, char *key, ...);
/*size_t JSON_get_array_size(struct Token *token, char *key, ...);*/
void destroy_token(struct Token *token);

#endif /* TOKEN_H */
