#ifndef TOKEN_H
#define TOKEN_H

#include <stddef.h>

#define MAX_JSON_KEY_LENGTH (63 + 1)
// 2 ** 64 + '\0'
#define MAX_NUMBER_STR_LENGTH 21

#define MAX_TOKEN_ERR_LENGTH (255 + 1)

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

char *get_token_err(struct Token *token);

int read_token_number(struct Token *token, char *key, ...);
char *rea_token_string(struct Token *token, char *key, ...);
size_t JSON_get_array_size(struct Token *token, char *key, ...);

void destroy_token(struct Token *token);

#endif /* TOKEN_H */
