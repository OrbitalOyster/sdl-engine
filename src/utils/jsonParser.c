#include "utils/jsonParser.h"

#include <stdio.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/wtree.h"

#define MAX_KEY_LENGTH 255
#define MAX_NUMBER_LENGTH 64
#define MAX_STRING_LENGTH 255

enum TokenType { Undefined, Object, Array, Number, String, Boolean, Eof };

union TokenValue {
  int number, boolean;
  char *string;
  struct TokenMap *map;
  void *empty;
};

struct Token {
  enum TokenType type;
  union TokenValue value;
};

struct TokenMap {
  unsigned int size;
  struct Token **content;
  struct WTree *tree;
};

struct TokenMap *createTokenMap() {
  struct TokenMap *result = calloc(1, sizeof(struct TokenMap));
  *result =
      (struct TokenMap){.size = 0, .content = NULL, .tree = createWTree()};
  return result;
}

void destroyToken(struct Token *token);

void destroyTokenMap(struct TokenMap *map) {
  destroyTree(map->tree);
  for (unsigned int i = 0; i < map->size; i++)
    destroyToken(map->content[i]);
  free(map->content);
  free(map);
}

struct Token *createToken(enum TokenType type, union TokenValue value) {
  struct Token *result = calloc(1, sizeof(struct Token));
  *result = (struct Token){.type = type, .value = value};
  return result;
}

void destroyToken(struct Token *token) {
  if (token->type == Object || token->type == Array)
    destroyTokenMap(token->value.map);
  free(token);
}

struct Token *createObjectToken() {
  struct TokenMap *map = createTokenMap();
  struct Token *result = createToken(Object, (union TokenValue){.map = map});
  return result;
}

struct Token *createArrayToken() {
  struct TokenMap *map = createTokenMap();
  struct Token *result = createToken(Array, (union TokenValue){.map = map});
  return result;
}

struct Token *createNumberToken(int n) {
  struct Token *result = createToken(Number, (union TokenValue){.number = n});
  return result;
}

struct Token *createStringToken(char *s) {
  struct Token *result = createToken(String, (union TokenValue){.string = s});
  return result;
}

void expandTokenMap(struct TokenMap *map, char *key, struct Token *token) {
  map->size++;
  map->content = realloc(map->content, map->size * sizeof(struct Token *));
  map->content[map->size - 1] = token;
  expandWTree(map->tree, key, map->content[map->size - 1]);
}

struct Token *readTokenMap(struct TokenMap *map, char *key) {
  struct Token *result = getWTreeEndpoint(map->tree, key);
  if (result == NULL)
    result = createToken(Undefined, (union TokenValue){.empty = NULL});
  return result;
}

char *tokenToString(struct Token *token);

char *tokenMapToString(struct TokenMap *map, int keys) {
  size_t size = 0;
  char *result = calloc(size, sizeof(char));
  for (unsigned int i = 0; i < map->tree->size; i++) {
    char *key = map->tree->words[i];
    char *value = tokenToString(map->content[i]);
    if (i) { // ", "
      size += 2 * sizeof(char);
      size += strlen(value) * sizeof(char);
      result = realloc(result, size);
      strcat(result, ", ");
    }
    if (keys) {
      result = realloc(result, strlen(key) + 2 * sizeof(char));
      strcat(result, key);
      strcat(result, ": ");
    }
    size += strlen(value);
    result = realloc(result, size);
    strcat(result, value);
  }
  return result;
}

char *arrayTokenToString(struct TokenMap *map) {
  char *s = tokenMapToString(map, 0);
  char *result = calloc(2 + strlen(s), sizeof(char));
  strcat(result, "[");
  strcat(result, s);
  strcat(result, "]");
  return result;
}

char *objectTokenToString(struct TokenMap *map) {
  char *s = tokenMapToString(map, 1);
  char *result = calloc(2 + strlen(s), sizeof(char));
  strcat(result, "{");
  strcat(result, s);
  strcat(result, "}");
  return result;
}

char *tokenToString(struct Token *token) {
  char *result = "";
  switch (token->type) {
  case Undefined:
    INFO("Undefined");
    break;
  case Object:
    INFO("{}");
    break;
  case Array:
    result = arrayTokenToString(token->value.map);
    break;
  case Number:
    result = calloc(MAX_NUMBER_LENGTH, sizeof(char));
    snprintf(result, MAX_NUMBER_LENGTH, "%i", token->value.number);
    break;
  case String:
    result = calloc(MAX_STRING_LENGTH, sizeof(char));
    snprintf(result, MAX_STRING_LENGTH, "\"%s\"", token->value.string);
    break;
  case Boolean:
    INFOF("Boolean: %i", token->value.boolean);
    break;
  case Eof:
    break;
  }
  return result;
}

void debugToken(struct Token *token) {
  switch (token->type) {
  case Undefined:
    INFO("Undefined");
    break;
  case Object:
    INFO("{}");
    break;
  case Array:
    INFO("[]");
    break;
  case Number:
    INFOF("Number: %i", token->value.number);
    break;
  case String:
    INFOF("String: %s", token->value.string);
    break;
  case Boolean:
    INFOF("Boolean: %i", token->value.boolean);
    break;
  case Eof:
    break;
  }
}

void processToken(enum TokenType type, FILE *f);

int isWhiteSpace(int c) { return (c == ' ' || c == '\n' || c == '\t'); }

int isDigit(int c) { return (c >= '0' && c <= '9'); }

enum TokenType identifyToken(int c) {
  enum TokenType result = Undefined;
  switch (c) {
  case EOF:
    result = Eof;
    break;
  case '{':
    result = Object;
    INFO("Token: Object");
    break;
  case '[':
    result = Array;
    break;
  case '"':
    result = String;
    INFO("Token: String");
    break;
  case 't':
  case 'f':
    result = Boolean;
    INFO("Token: Boolean");
    break;
  default:
    if (isDigit(c)) {
      if (c == '0')
        ERR(1, "Invalid number format");
      result = Number;
      INFO("Token: Number");
    }
    break;
  }
  return result;
}

void readKey(FILE *f) {
  char *key = calloc(MAX_KEY_LENGTH, sizeof(int));
  int c, n = 0;
  int done = 0;
  do {
    c = fgetc(f);
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    if (c == '"')
      done = 1;
    else
      key[n++] = (char)c;
  } while (!done);
  INFOF("Read key: %s", key);
}

void readString(FILE *f) {
  char *string = calloc(MAX_STRING_LENGTH, sizeof(int));
  int c, n = 0;
  int done = 0;
  do {
    c = fgetc(f);
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    if (c == '"')
      done = 1;
    else
      string[n++] = (char)c;
  } while (!done);
  INFOF("Read String: %s", string);
}

void readNumber(FILE *f) {
  char *number = calloc(MAX_STRING_LENGTH, sizeof(int));
  int c, n = 0;
  int done = 0;
  fseek(f, -1, SEEK_CUR);
  do {
    c = fgetc(f);
    if (!isDigit(c))
      done = 1;
    else
      number[n++] = (char)c;
  } while (!done);
  INFOF("Read Number: %s", number);
  fseek(f, -1, SEEK_CUR);
}

void readBoolean(FILE *f) {
  char *boolean = calloc(5, sizeof(int));
  int c, n = 0;
  int done = 0;
  int readingTrue = 0;
  const char *trueString = "true";
  const char *falseString = "false";
  fseek(f, -1, SEEK_CUR);
  do {
    c = fgetc(f);
    INFOF("Read char: %i [%c]", c, c);
    if (!n && c == 't')
      readingTrue = 1;
    boolean[n] = (char)c;
    if (readingTrue) {
      if (c != trueString[n])
        ERRF(1, "Expected 'true', got '%c'", c);
      if (n == 3)
        done = 1;
    } else {
      if (c != falseString[n])
        ERRF(1, "Expected 'false', got '%c'", c);
      if (n == 4)
        done = 1;
    }
    n++;
  } while (!done);
  INFOF("Read Boolean: %s", boolean);
}

int findNextKey(FILE *f, int expectingComma) {
  int c;
  int done = 0;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == ',') {
      if (!expectingComma)
        ERR(1, "Unexpected ','");
      expectingComma = 0;
      continue;
    }
    if (c == '}')
      return 0;
    if (c != '"')
      ERRF(1, "Expected key, got [%c]", c);
    done = 1;
  } while (!done);
  return 1;
}

void findNextColon(FILE *f) {
  int c;
  int done = 0;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    if (c != ':')
      ERRF(1, "Expected ':', got [%c]", c);
    done = 1;
  } while (!done);
  INFO("Found colon");
}

enum TokenType findNextToken(FILE *f, int expectingComma) {
  enum TokenType nextToken = Undefined;
  int c;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == ',') {
      if (!expectingComma)
        ERR(1, "Unexpected ','");
      expectingComma = 0;
      continue;
    }
    INFOF("Read char: %i [%c]", c, c);
    nextToken = identifyToken(c);
    if (nextToken == Undefined)
      ERR(1, "Unexected character");
  } while (nextToken == Undefined);

  return nextToken;
}

void processObject(FILE *f) {
  int done = 0;
  int expectingComma = 0;
  do {
    if (!findNextKey(f, expectingComma))
      done = 1;
    else {
      readKey(f);
      findNextColon(f);
      enum TokenType nextToken = findNextToken(f, 0);
      processToken(nextToken, f);
      expectingComma = 1;
    }
  } while (!done);
}

int processNextArrayElement(FILE *f, int expectingComma) {
  int c;
  int done = 0;
  enum TokenType nextElement = Undefined;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    if (c == ',') {
      if (!expectingComma)
        ERR(1, "Unexpected ','");
      expectingComma = 0;
      continue;
    }
    if (c == ']')
      return 0;
    INFOF("Read char: %i [%c]", c, c);
    nextElement = identifyToken(c);
    processToken(nextElement, f);
    done = 1;
  } while (!done);
  return 1;
}

void processArray(FILE *f) {
  int done = 0;
  int expectingComma = 0;
  do {
    if (!processNextArrayElement(f, expectingComma))
      done = 1;
    else
      expectingComma = 1;
  } while (!done);
}

void processToken(enum TokenType type, FILE *f) {
  switch (type) {
  case Undefined:
    break;
  case Object:
    INFO("Processing Object...");
    processObject(f);
    break;
  case Array:
    INFO("Processing Array...");
    processArray(f);
    break;
  case Number:
    INFO("Processing Number...");
    readNumber(f);
    break;
  case String:
    INFO("Processing String...");
    readString(f);
    break;
  case Boolean:
    INFO("Processing Boolean...");
    readBoolean(f);
    break;
  case Eof:
    fclose(f);
    break;
  }
}

void readFile(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  enum TokenType nextToken = findNextToken(f, 0);
  processToken(nextToken, f);
}

void tokenTest() {
  struct TokenMap *map = createTokenMap();
  struct Token *intToken =
      createToken(Number, (union TokenValue){.number = 14});
//  struct Token *strToken =
//      createToken(String, (union TokenValue){.string = "Hello"});
  expandTokenMap(map, "num", intToken);
//  expandTokenMap(map, "str", strToken);
  destroyTokenMap(map);
  /*
  struct Token *arrToken = createArrayToken();
  expandTokenMap(map, "arr", arrToken);

  printf("Token to string: %s\n", tokenToString(intToken));
  printf("Token to string: %s\n", tokenToString(strToken));
  printf("Token map to string: %s\n", tokenMapToString(map, 1));
  printf("Array token to string: %s\n", arrayTokenToString(map));
  printf("Object token to string: %s\n", objectTokenToString(map));
  */
}
