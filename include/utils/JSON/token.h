#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#define MAX_NUMBER_LENGTH 64
#define MAX_JSON_KEY_LENGTH (63 + 1)

struct Token;

enum TokenType { Undefined, Object, Array, Number, String, Boolean, Null };

union TokenValue {
  int number, boolean;
  char *string;
  struct TokenMap *map;
  struct TokenArray *array;
};

struct Token *create_token(enum TokenType type, union TokenValue value);

enum TokenType get_token_type(struct Token *token);
const char *token_type_to_string(int t);
union TokenValue get_token_value(struct Token *token);

int get_JSON_foo_number(struct Token *token, char *key, ...);
char *get_JSON_foo_string(struct Token *token, char *key, ...);
size_t JSON_get_array_size(struct Token *token, char *key, ...);

void destroy_token(struct Token *token);

#endif /* TOKEN_H */
