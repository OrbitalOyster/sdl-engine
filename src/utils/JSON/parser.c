#include "utils/JSON/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/debug.h"

const char *TRUE_STRING = "true";
const char *FALSE_STRING = "false";
const char *NULL_STRING = "null";

static struct Token *parse_token_F(FILE *f, int c);

static int is_whitespace(int c) { return (c == ' ' || c == '\n' || c == '\t'); }

static int is_digit(int c) { return (c >= '0' && c <= '9'); }

static enum TokenType identify_token(int c) {
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
    if (is_digit(c)) {
      if (c == '0')
        ERR(1, "Invalid number format");
      result = Number;
      INFO("Token: Number");
    }
    break;
  }
  return result;
}

static char *read_string_F(FILE *f) {
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

static int read_number_F(FILE *f) {
  char *number = calloc(MAX_STRING_LENGTH, sizeof(int));
  int c, n = 0;
  int done = 0;
  fseek(f, -1, SEEK_CUR);
  do {
    c = fgetc(f);
    if (!is_digit(c))
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

static int read_boolean_F(FILE *f) {
  int c, n = 0, done = 0, reading_true = 0;
  fseek(f, -1, SEEK_CUR);
  do {
    c = fgetc(f);
    INFOF("Read char: %i [%c]", c, c);
    if (!n && c == 't')
      reading_true = 1;
    if (reading_true) {
      if (c != TRUE_STRING[n])
        ERRF(1, "Expected 'true', got '%c'", c);
      if (n == 3)
        done = 1;
    } else {
      if (c != FALSE_STRING[n])
        ERRF(1, "Expected 'false', got '%c'", c);
      if (n == 4)
        done = 1;
    }
    n++;
  } while (!done);
  INFOF("Read Boolean: %i", reading_true);
  return reading_true;
}

static void read_null_F(FILE *f) {
  int c;
  fseek(f, -1, SEEK_CUR);
  for (int i = 0; i < 4; i++) {
    c = fgetc(f);
    INFOF("Read char: %i [%c]", c, c);
    if (c != NULL_STRING[i])
      ERRF(1, "Expected 'null', got '%c'", c);
  }
}

static int skip_whitespaces_F(FILE *f) {
  int c;
  do {
    c = fgetc(f);
  } while (is_whitespace(c));
  return c;
}

static int skip_to_next_object_token_F(FILE *f) {
  int c = skip_whitespaces_F(f);
  // Comma or '}'
  switch (c) {
  case ',':
    // Must be '"'
    c = skip_whitespaces_F(f);
    if (c != '"')
      ERRF(1, "Expected key, got [%c]", c);
    return c;
  case '}':
    // Done
    return c;
  case EOF:
    ERR(1, "Unexpected EOF");
  default:
    // Error
    ERRF(1, "Expected ',' or '}', got [%c]", c);
  }
}

static struct TokenMap *parse_object_F(FILE *f) {
  struct TokenMap *result = create_token_map();
  int c = skip_whitespaces_F(f);
  while (c != '}') {
    // Key
    if (c != '"')
      ERRF(1, "Expected key, got [%c]", c);
    char *key = read_string_F(f);
    c = skip_whitespaces_F(f);
    // Colon
    if (c != ':')
      ERRF(1, "Expected ':', got [%c]", c);
    c = skip_whitespaces_F(f);
    // Value
    struct Token *new_token = parse_token_F(f, c);
    // Add to object
    expand_token_map(result, key, new_token);
    free(key);
    c = skip_to_next_object_token_F(f);
  }
  return result;
}

static int skip_to_next_array_token_F(FILE *f) {
  int c = skip_whitespaces_F(f);
  // Comma or ']'
  switch (c) {
  case ',':
    // Must not be ',' or ']'
    c = skip_whitespaces_F(f);
    if (c == ',' || c == ']')
      ERRF(1, "Expected value, got [%c]", c);
    return c;
  case ']':
    // Done
    return c;
  case EOF:
    ERR(1, "Unexpected EOF");
  default:
    // Error
    ERRF(1, "Expected ',' or ']', got [%c]", c);
  }
}

static struct TokenMap *parse_array_F(FILE *f) {
  struct TokenMap *result = create_token_map();
  int c = skip_whitespaces_F(f);
  while (c != ']') {
    struct Token *next_token = parse_token_F(f, c);
    expandTokenMapN(result, next_token);
    c = skip_to_next_array_token_F(f);
  }
  return result;
}

static struct Token *parse_token_F(FILE *f, int c) {
  union TokenValue value;
  enum TokenType type = identify_token(c);
  switch (type) {
  case Undefined:
    ERRF(1, "Unexpected character: %i [%c]", c, c);
    break;
  case Object:
    INFO("Processing Object...");
    value.map = parse_object_F(f);
    break;
  case Array:
    INFO("Processing Array...");
    value.map = parse_array_F(f);
    break;
  case Number:
    INFO("Processing Number...");
    value.number = read_number_F(f);
    break;
  case String:
    INFO("Processing String...");
    value.string = read_string_F(f);
    break;
  case Boolean:
    INFO("Processing Boolean...");
    value.boolean = read_boolean_F(f);
    break;
  case Null:
    INFO("Processing Null...");
    read_null_F(f);
    break;
  }
  struct Token *result = create_token(type, value);
  return result;
}

struct Token *read_json_file(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  int c = skip_whitespaces_F(f);
  struct Token *result = parse_token_F(f, c);
  fclose(f);
  return result;
}
