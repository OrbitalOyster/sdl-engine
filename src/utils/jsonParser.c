#include "utils/jsonParser.h"

#include <stdio.h>

#include "utils/wtree.h"
#include "utils/debug.h"

#define MAX_KEY_LENGTH 255
#define MAX_STRING_LENGTH 255

enum TokenType { Undefined, Object, Array, Number, String, Boolean, Eof };

union TokenValue {
  int number;
  char *string;
  int boolean;
  struct TokenMap *map;
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

struct Token createToken(enum TokenType type, union TokenValue value) {
  return (struct Token) {.type = type, .value = value};
}

struct TokenMap createTokenMap() {
  struct TokenMap result = {.size = 0, .content = NULL, .tree = createWTree()};
  return result;
}

void expandTokenMap(struct TokenMap *map, char *key, struct Token *token) {
  map->size++;
  map->content = realloc(map->content, map->size * sizeof(struct Token*));
  map->content[map->size - 1] = token;
  expandWTree(map->tree, key, map->content[map->size - 1]);
}

struct Token *readTokenMap(struct TokenMap *map, char *key) {
  struct Token *result = getWTreeEndpoint(map->tree, key);
  return result;
}

void debugToken(struct Token *token) {
  switch (token->type) {
  case Undefined:
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

struct JSON {
  struct Token root;
};

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
  struct TokenMap map = createTokenMap();
  struct Token intToken = createToken(Number, (union TokenValue) {.number = 14});
  struct Token strToken = createToken(String, (union TokenValue) {.string = "Hello"});
  expandTokenMap(&map, "n", &intToken);
  expandTokenMap(&map, "s", &strToken);

  struct Token *n = readTokenMap(&map, "n");
  struct Token *s = readTokenMap(&map, "s");
  debugToken(n);
  debugToken(s);
}
