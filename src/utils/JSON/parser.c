#include "utils/JSON/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/debug.h"

#define INITIAL_STRING_LENGTH 8

static int is_whitespace(int c) { return (c == ' ' || c == '\n' || c == '\t'); }

static int is_digit(int c) { return (c >= '0' && c <= '9'); }

static char *read_string(void *json, int (*get_next_char)(void *json)) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *string = calloc(l, sizeof(char));
  int esc = 0, c, done = 0;
  do {
    c = get_next_char(json);
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    // Check if we're done
    if (c == '"' && !esc)
      done = 1;
    else
      string[n++] = (char)c;
    // Check string size
    if (n == l - 1) {
      l *= 2;
      string = realloc(string, sizeof(char) * l);
    }
    // Check for ESC sequence
    esc = (c == '\\' && !esc);
  } while (!done);
  string[n] = '\0';
  INFOF("Read String: %s", string);
  return string;
}

static int read_number(void *json, int (*get_next_char)(void *json), void (*rewind)(void *json)) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *number = calloc(l, sizeof(char));
  int c, done = 0;
  rewind(json);

  do {
    c = get_next_char(json);
    if (!is_digit(c))
      done = 1;
    else
      number[n++] = (char)c;
    // Check string size
    if (n == l - 1) {
      l *= 2;
      number = realloc(number, sizeof(char) * l);
    }
  } while (!done);

  number[n] = '\0';
  INFOF("Read Number: %s", number);
  int result = atoi(number);
  INFOF("Actual number: %s", number);
  free(number);

  rewind(json);

  return result;
}

static int read_boolean(void *json, int (*get_next_char)(void *json), void (*rewind)(void *json)) {
  int c, n = 0, done = 0, reading_true = 0;
  
  rewind(json);

  do {
    c = get_next_char(json);
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

static void read_null(void *json, int (*get_next_char)(void *json), void (*rewind)(void *json)) {
  int c;
  rewind(json);
  for (int i = 0; i < 4; i++) {
    c = get_next_char(json);
    INFOF("Read char: %i [%c]", c, c);
    if (c != NULL_STRING[i])
      ERRF(1, "Expected 'null', got '%c'", c);
  }
}

void skip_whitespaces(void *json, int (*get_next_char)(void *json),
                      void (*rewind)(void *json)) {
  INFO("Skipping whitespaces...");
  int c;
  do {
    c = get_next_char(json);
  } while (is_whitespace(c));
  rewind(json);
}

static int skip_to_next_object_token(void *json,
                                     int (*get_next_char)(void *json),
                                     void (*rewind)(void *json)) {
  INFO("Skipping to next object token");
  skip_whitespaces(json, get_next_char, rewind);
  int c = get_next_char(json);
  // Comma or '}'
  switch (c) {
  case ',':
    // Must be '"'
    skip_whitespaces(json, get_next_char, rewind);
    c = get_next_char(json);
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

static struct TokenMap *parse_object(void *json,
                                     int (*get_next_char)(void *json),
                                     void (*rewind)(void *json)) {
  struct TokenMap *result = create_token_map();
  skip_whitespaces(json, get_next_char, rewind);
  int c = get_next_char(json);
  while (c != '}') {
    INFO("Reading next key-value pair");
    // Key
    if (c != '"')
      ERRF(1, "Expected key, got [%c]", c);
    char *key = read_string(json, get_next_char);
    skip_whitespaces(json, get_next_char, rewind);
    c = get_next_char(json);
    // Colon
    if (c != ':')
      ERRF(1, "Expected ':', got [%c]", c);
    skip_whitespaces(json, get_next_char, rewind);
    // c = get_next_char(json);
    // Value
    struct Token *new_token = parse_token(json, get_next_char, rewind);
    // Add to object
    expand_token_map(result, key, new_token);
    free(key);
    c = skip_to_next_object_token(json, get_next_char, rewind);
    // c = get_next_char(json);
  }
  return result;
}

static int skip_to_next_array_token(void *json, int (*get_next_char)(void *json), void (*rewind)(void *json)) {
  INFO("Skipping to next array token...");
  skip_whitespaces(json, get_next_char, rewind);
  int c = get_next_char(json);
  // Comma or ']'
  switch (c) {
  case ',':
    // Must not be ',' or ']'
    skip_whitespaces(json, get_next_char, rewind);
    c = get_next_char(json);
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

static struct TokenMap *parse_array(void *json, int (*get_next_char)(void *json), void (*rewind)(void *json)) {
  INFO("Parsing array...");
  struct TokenMap *result = create_token_map();
  skip_whitespaces(json, get_next_char, rewind);
  // int c = -1; //get_next_char(json);
  int c = get_next_char(json);
  while (c != ']') {
    INFO("Reading next array token");
    // skip_whitespaces(json, get_next_char, rewind);
    rewind(json);
    struct Token *next_token = parse_token(json, get_next_char, rewind);
    expandTokenMapN(result, next_token);
    c = skip_to_next_array_token(json, get_next_char, rewind);
  }
  return result;
}

struct Token *parse_token(void *json, int (*get_next_char)(void *json),
                                 void (*rewind)(void *json)) {
  enum TokenType type = Undefined;
  union TokenValue value;
  int c = get_next_char(json);
  switch (c) {
//  case Undefined:
//    ERRF(1, "Unexpected character: %i [%c]", c, c);
//    break;
  case '{':
    INFO("Processing Object...");
    type = Object; 
    value.map = parse_object(json, get_next_char, rewind);
    break;
  case '[':
    INFO("Processing Array...");
    type = Array; 
    value.map = parse_array(json, get_next_char, rewind);
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    INFO("Processing Number...");
    type = Number; 
    value.number = read_number(json, get_next_char, rewind);
    break;
  case '\"':
    INFO("Processing String...");
    type = String; 
    value.string = read_string(json, get_next_char);
    break;
  case 't':
  case 'f':
    INFO("Processing Boolean...");
    type = Boolean; 
    value.boolean = read_boolean(json, get_next_char, rewind);
    break;
  case 'n':
    INFO("Processing Null...");
    type = Null; 
    read_null(json, get_next_char, rewind);
    break;
  default:
    ERRF(1, "Unexpected character: %i [%c]", c, c);
    break;
  }
  struct Token *result = create_token(type, value);
  return result;
}
