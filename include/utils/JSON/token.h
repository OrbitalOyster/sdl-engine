#ifndef TOKEN_H
#define TOKEN_H

#define MAX_KEY_LENGTH 255
#define MAX_NUMBER_LENGTH 64
#define MAX_STRING_LENGTH 255

struct Token;

enum TokenType { Undefined, Object, Array, Number, String, Boolean, Null };

union TokenValue {
  int number, boolean;
  char *string;
  struct TokenMap *map;
};

struct Token *create_token(enum TokenType type, union TokenValue value);
enum TokenType get_token_type(struct Token *token);
union TokenValue get_token_value(struct Token *token);
struct Token *create_undefined_token();
struct Token *create_object_token();
struct Token *create_array_token();
struct Token *create_number_token(int n);
struct Token *create_string_token(char *s);
struct Token *create_boolean_token(int b);
struct Token *create_null_token();
unsigned int get_array_token_size(struct Token *arr);
struct Token *get_array_token_element(struct Token *arr, unsigned int n);
void expand_object_token(struct Token *obj, char *key, struct Token *token);
void expand_array_token(struct Token *arr, struct Token *token);
void destroy_token(struct Token *token);

#endif /* TOKEN_H */
