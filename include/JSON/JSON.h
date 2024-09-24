#ifndef JSON_H
#define JSON_H

#define MAX_TOKEN_KEY_LENGTH (63 + 1)
#define MAX_NUMBER_STR_LENGTH 21 // 2 ** 64 + '\0'
#define MAX_TOKEN_ERR_LENGTH (255 + 1)

struct Token;
enum TokenType { Undefined, Object, Array, Number, String, Boolean, Null };

union TokenValue {
  int number, boolean;
  char *string;
  struct TokenMap *map;
  struct TokenArray *array;
};

struct TokenArray;
struct TokenMap;
struct JSON_Parser;

#endif /* JSON_H */
