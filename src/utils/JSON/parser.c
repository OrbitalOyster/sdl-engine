#include "utils/JSON/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/debug.h"

#define TRUE_STRING "true"
#define TRUE_STRING_0 't'
#define TRUE_STRING_LENGTH 4

#define FALSE_STRING "false"
#define FALSE_STRING_0 'f'
#define FALSE_STRING_LENGTH 5

#define NULL_STRING "null"
#define NULL_STRING_0 'n'
#define NULL_STRING_LENGTH 4

#define INITIAL_STRING_LENGTH 8

static int is_digit(int c) { return (c >= '0' && c <= '9'); }

static char *read_string(void *json, int (*get_next_char)(void *json)) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *string = calloc(l, sizeof(char));
  int esc = 0, c, done = 0;
  do {
    c = get_next_char(json);
    if (c == EOF) {
      set_JSON_err(json, "Unexpected EOF");
      free(string);
      return NULL;
    }
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
  INFOF("Read string: %s", string);
  return string;
}

static int read_number(void *json, int (*get_next_char)(void *json),
                       void (*rewind)(void *json)) {
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

static void read_true(void *json, int (*get_next_char)(void *json)) {
  int c;
  for (unsigned int i = 1; i < TRUE_STRING_LENGTH; i++) {
    c = get_next_char(json);
    if (c != TRUE_STRING[i]) {
      set_JSON_err(json, "Expected \"true\"");
      return;
//      ERRF(1, "Expected true, got %c[%i]", c, c);
    }
  }
}

static void read_false(void *json, int (*get_next_char)(void *json)) {
  int c;
  for (unsigned int i = 1; i < FALSE_STRING_LENGTH; i++) {
    c = get_next_char(json);
    if (c != FALSE_STRING[i]) {
      set_JSON_err(json, "Expected \"false\"");
      return;
//      ERRF(1, "Expected true, got %c[%i]", c, c);
    }
  }
}

static void read_null(void *json, int (*get_next_char)(void *json)) {
  int c;
  for (unsigned int i = 1; i < NULL_STRING_LENGTH; i++) {
    c = get_next_char(json);
    if (c != NULL_STRING[i]) {
      set_JSON_err(json, "Expected \"null\"");
      return;
//      ERRF(1, "Expected null, got %c[%i]", c, c);
    }
  }
}

int next_non_whitespace(void *json, int (*get_next_char)(void *json)) {
  INFO("Skipping to non-whitespace...");
  int c;
  while (1) {
    switch (c = get_next_char(json)) {
    case ' ':
    case '\t':
    case '\n':
      break;
    default:
      return c;
    }
  }
}

static int skip_to_next_object_token(void *json,
                                     int (*get_next_char)(void *json)) {
  INFO("Skipping to next object token");
  int c = next_non_whitespace(json, get_next_char);
  // Comma or '}'
  switch (c) {
  case ',':
    // Must be '"'
    c = next_non_whitespace(json, get_next_char);
    if (c != '"') {
      set_JSON_err(json, "Expected key");
      return 0;
      // ERRF(1, "Expected key, got [%c]", c);
    }
    return c;
  case '}':
    // Done
    return c;
  case EOF: {
    set_JSON_err(json, "Unexpected EOF");
    return 0;
    // ERR(1, "Unexpected EOF");
  }
  default:
    // Error
    set_JSON_err(json, "Expected ',' or '}'");
    // ERRF(1, "Expected ',' or '}', got [%c]", c);
    return 0;
  }
}

static struct TokenMap *parse_object(void *json,
                                     int (*get_next_char)(void *json),
                                     void (*rewind)(void *json)) {
  struct TokenMap *result = create_token_map();
  int c = next_non_whitespace(json, get_next_char);
  while (c != '}') {
    INFO("Reading next key-value pair");
    // Key
    if (c != '"') {
      set_JSON_err(json, "Expected key");
//      ERRF(1, "Expected key, got [%c]", c);
      return result;
    }
    char *key = read_string(json, get_next_char);

    if (get_JSON_err(json))
      return result;

    // Colon
    c = next_non_whitespace(json, get_next_char);
    if (c != ':') {
      set_JSON_err(json, "Expected ':'");
      // ERRF(1, "Expected ':', got [%c]", c);
      return result;
    }
    // Value
    struct Token *new_token = parse_next_token(json, get_next_char, rewind);

    if (get_JSON_err(json))
      return result;

    // Add to object
    expand_token_map(result, key, new_token);
    free(key);
    c = skip_to_next_object_token(json, get_next_char);

    if (get_JSON_err(json))
      return result;
  }
  return result;
}

static int skip_to_next_array_token(void *json,
                                    int (*get_next_char)(void *json)) {
  INFO("Skipping to next array token...");
  int c = next_non_whitespace(json, get_next_char);
  // Comma or ']'
  switch (c) {
  case ',':
    c = next_non_whitespace(json, get_next_char);
    // Must not be ',' or ']'
    if (c == ',' || c == ']') {
      set_JSON_err(json, "Expected value");
//      ERRF(1, "Expected value, got [%c]", c);
    }
    return c;
  case ']':
    // Done
    return c;
  case EOF:
    set_JSON_err(json, "Unexpected EOF");
    return 0;
//    ERR(1, "Unexpected EOF");
  default:
    // Error {
    set_JSON_err(json, "Expected ',' or ']'");
    return 0;
//    ERRF(1, "Expected ',' or ']', got [%c]", c);
  }
}

static struct TokenMap *parse_array(void *json,
                                    int (*get_next_char)(void *json),
                                    void (*rewind)(void *json)) {
  INFO("Parsing array...");
  struct TokenMap *result = create_token_map();
  int c = next_non_whitespace(json, get_next_char);
  while (c != ']') {
    INFO("Reading next array token");
    rewind(json);
    struct Token *next_token = parse_next_token(json, get_next_char, rewind);

    if (get_JSON_err(json))
      return result;

    expandTokenMapN(result, next_token);
    c = skip_to_next_array_token(json, get_next_char);

    if (get_JSON_err(json))
      return result;
  }
  return result;
}

struct Token *parse_next_token(void *json, int (*get_next_char)(void *json),
                               void (*rewind)(void *json)) {
  enum TokenType type = Undefined;
  union TokenValue value;
  int c = next_non_whitespace(json, get_next_char);
  switch (c) {
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
  case TRUE_STRING_0:
    INFO("Processing Boolean (true)...");
    read_true(json, get_next_char);
    type = Boolean;
    value.boolean = 1;
    break;
  case FALSE_STRING_0:
    INFO("Processing Boolean (false)...");
    read_false(json, get_next_char);
    type = Boolean;
    value.boolean = 0;
    break;
  case NULL_STRING_0:
    INFO("Processing Null...");
    type = Null;
    read_null(json, get_next_char);
    break;
  default:
    ERRF(1, "Unexpected character: %i [%c]", c, c);
    break;
  }
  struct Token *result = create_token(type, value);
  return result;
}
