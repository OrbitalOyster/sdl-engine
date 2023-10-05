#ifndef TOKEN_H
#define TOKEN_H

#define MAX_KEY_LENGTH 255
#define MAX_NUMBER_LENGTH 64
#define MAX_STRING_LENGTH 255

struct Token;

enum TokenType { Undefined, Object, Array, Number, String, Boolean };

union TokenValue {
  int number, boolean;
  char *string;
  struct TokenMap *map;
  void *empty;
};

struct Token *createToken(enum TokenType type, union TokenValue value);
enum TokenType getTokenType(struct Token *token);
union TokenValue getTokenValue(struct Token *token);
struct Token *createObjectToken();
struct Token *createArrayToken();
struct Token *createNumberToken(int n);
struct Token *createStringToken(char *s);
struct Token *createBooleanToken(int b);
unsigned int getArrayTokenSize(struct Token *arr);
struct Token *getArrayTokenElement(struct Token *arr, unsigned int n);
void expandObjectToken(struct Token *obj, char *key, struct Token *token);
void expandArrayToken(struct Token *arr, struct Token *token);
char *objectTokenToString(struct TokenMap *map);
char *tokenToString(struct Token *token);
void destroyToken(struct Token *token);

#endif
