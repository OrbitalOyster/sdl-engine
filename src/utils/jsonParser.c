#include "utils/jsonParser.h"

#include <stdio.h>

#include "utils/debug.h"

enum Token { Undefined, Object, Array, Number, String, Boolean, Eof };

#define MAX_KEY_LENGTH 255
#define MAX_STRING_LENGTH 255

void processToken(enum Token type, FILE *f);

int isWhiteSpace(int c) { return (c == ' ' || c == '\n' || c == '\t'); }

int isDigit(int c) { return (c >= '0' && c <= '9'); }

enum Token identifyToken(int c) {
  enum Token result = Undefined;
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
  default:
    if (isDigit(c)) {
      if (c == '0')
        ERR(1, "Invalid number format");
      result = Number;
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

enum Token findNextToken(FILE *f, int expectingComma) {
  enum Token nextToken = Undefined;
  int c;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == ',') {
      if (!expectingComma) ERR( 1, "Unexpected ','");
      expectingComma = 0;
      continue;
    }
    INFOF("Read char: %i (%c)", c, c);
    nextToken = identifyToken(c);
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
      enum Token nextToken = findNextToken(f, 0);
      processToken(nextToken, f);
      expectingComma = 1;
    }
  } while (!done);
}

int findNextArrayElement(FILE* f, int expectingComma) {
  int c;
  int done = 0;
  do {
    c = fgetc(f);
    if (isWhiteSpace(c))
      continue;
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    if (c == ',') {
      if (!expectingComma) ERR(1, "Unexpected ','");
      expectingComma = 0;
      continue;
    }
    if (c == ']') return 0;
    INFOF("Read char: %i (%c)", c, c);
    done = 1;
  } while (!done);
  return 1;
}

void processArray(FILE *f) {
  int done = 0;
  int comma = 0;
  enum Token nextElement = Undefined;
  do {
    if (!findNextArrayElement(f, comma)) done = 1;
    else {
      comma = 1;
      nextElement = identifyToken(c);
    }

  } while (!done);
}

void processToken(enum Token type, FILE *f) {
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
    break;
  case Eof:
    fclose(f);
    break;
  }
}

void readFile(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  enum Token nextToken = findNextToken(f, 0);
  processToken(nextToken, f);
}
