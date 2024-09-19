#ifndef TOKEN_H
#define TOKEN_H

#define MAX_NUMBER_LENGTH 64

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

void destroy_token(struct Token *token);

#endif /* TOKEN_H */
