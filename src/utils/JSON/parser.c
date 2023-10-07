#include "utils/JSON/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/debug.h"

const char *trueString = "true";
const char *falseString = "false";
const char *nullString = "null";

struct Token *parseTokenF(FILE *f, int c);

int isWhiteSpace(int c) { return (c == ' ' || c == '\n' || c == '\t'); }

int isDigit(int c) { return (c >= '0' && c <= '9'); }

enum TokenType identifyToken(int c) {
  enum TokenType result = Undefined;
  switch (c) {
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
  case 'n':
    result = Null;
    INFO("Token: Null");
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

char *readStringF(FILE *f) {
  char *string = calloc(MAX_STRING_LENGTH, sizeof(char));
  int c, n = 0, done = 0;
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
  return string;
}

int readNumberF(FILE *f) {
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
  int result = atoi(number);
  INFOF("Actual number: %s", number);
  free(number);
  fseek(f, -1, SEEK_CUR);
  return result;
}

int readBooleanF(FILE *f) {
  int c, n = 0, done = 0, readingTrue = 0;
  fseek(f, -1, SEEK_CUR);
  do {
    c = fgetc(f);
    INFOF("Read char: %i [%c]", c, c);
    if (!n && c == 't')
      readingTrue = 1;
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
  INFOF("Read Boolean: %i", readingTrue);
  return readingTrue;
}

void readNullF(FILE *f) {
  int c;
  fseek(f, -1, SEEK_CUR);
  for (int i = 0; i < 4; i++) {
    c = fgetc(f);
    INFOF("Read char: %i [%c]", c, c);
    if (c != nullString[i]) ERRF(1, "Expected 'null', got '%c'", c);
  }
}

int skipWhiteSpacesF(FILE *f, int skipComma) {
  int c;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == ',') {
      if (!skipComma)
        ERR(1, "Unexpected ','");
      skipComma = 0;
      continue;
    }
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    return c;
  } while (1);
}

struct TokenMap *parseObjectF(FILE *f) {
  struct TokenMap *result = createTokenMap();
  int done = 0;
  int expectingComma = 0;
  int c;
  do {
    c = skipWhiteSpacesF(f, expectingComma);
    if (c == '}')
      done = 1;
    else {
      // Key
      if (c != '"')
        ERRF(1, "Expected key, got [%c]", c);
      char *key = readStringF(f);
      c = skipWhiteSpacesF(f, 0);
      // Colon
      if (c != ':')
        ERRF(1, "Expected ':', got [%c]", c);
      c = skipWhiteSpacesF(f, 0);
      // Value
      struct Token *newToken = parseTokenF(f, c);
      // Add to object
      expandTokenMap(result, key, newToken);
      free(key);
      expectingComma = 1;
    }
  } while (!done);
  return result;
}

struct Token *parseNextArrayElementF(FILE *f, int expectingComma) {
  int c = skipWhiteSpacesF(f, expectingComma);
  if (c == ']')
    return NULL;
  INFOF("Read char: %i [%c]", c, c);
  struct Token *token = parseTokenF(f, c);
  return token;
}

struct TokenMap *parseArrayF(FILE *f) {
  struct TokenMap *result = createTokenMap();
  int done = 0;
  int expectingComma = 0;
  do {
    struct Token *nextToken = parseNextArrayElementF(f, expectingComma);
    if (nextToken == NULL)
      done = 1;
    else {
      expandTokenMapN(result, nextToken);
      expectingComma = 1;
    }
  } while (!done);
  return result;
}

struct Token *parseTokenF(FILE *f, int c) {
  union TokenValue value;
  enum TokenType type = identifyToken(c);
  switch (type) {
  case Undefined:
    ERRF(1, "Unexpected character: %i [%c]", c, c);
    break;
  case Object:
    INFO("Processing Object...");
    value.map = parseObjectF(f);
    break;
  case Array:
    INFO("Processing Array...");
    value.map = parseArrayF(f);
    break;
  case Number:
    INFO("Processing Number...");
    value.number = readNumberF(f);
    break;
  case String:
    INFO("Processing String...");
    value.string = readStringF(f);
    break;
  case Boolean:
    INFO("Processing Boolean...");
    value.boolean = readBooleanF(f);
    break;
   case Null:
    INFO("Processing Null...");
    readNullF(f);
    break;
  }
  struct Token *result = createToken(type, value);
  return result;
}

struct Token *readFile(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  int c = skipWhiteSpacesF(f, 0);
  struct Token *result = parseTokenF(f, c);
  fclose(f);
  return result;
}
