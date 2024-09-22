#ifndef JSON_H
#define JSON_H

#define MAX_JSON_KEY_LENGTH (63 + 1)
// 2 ** 64 + '\0'
#define MAX_NUMBER_STR_LENGTH 21

#define MAX_TOKEN_ERR_LENGTH (255 + 1)

struct Token;
union TokenValue;
struct TokenArray;
struct TokenMap;
struct JSON_Parser;

#endif /* JSON_H */