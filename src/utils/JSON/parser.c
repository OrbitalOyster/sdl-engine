#include "utils/JSON/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"
#include "utils/JSON/token.h"

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

#define INITIAL_STRING_LENGTH 32

static int is_digit(int c) { return (c >= '0' && c <= '9'); }

static char *read_string(struct JSON *json,
                         int (*get_next_char)(struct JSON *, int)) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *string = calloc(l, sizeof(char));
  int esc = 0, c, done = 0;
  do {
    c = get_next_char(json, 0);
    switch (c) {
    // No unexpected EOF
    case EOF:
      set_JSON_err(json, "Unexpected EOF");
      free(string);
      return NULL;
    // No multiline strings
    case '\n':
      set_JSON_err(json, "Unexpected \\n");
      free(string);
      return NULL;
    // Check if we're done
    case '"':
      done = !esc; // Ignore [\"], finish parasing on ["]
      break;
    }
    if (!done) {
      string[n++] = (char)c;
      // Check string size
      if (n == l - 1) {
        l *= 2;
        string = realloc(string, sizeof(char) * l);
      }
      // Check for ESC sequence
      esc = (c == '\\' && !esc);
    }
  } while (!done);
  string[n] = '\0';
  INFOF("Read string: %s", string);
  return string;
}

static int read_number(int c1, struct JSON *json,
                       int (*get_next_char)(struct JSON*, int)) {
  size_t l = INITIAL_STRING_LENGTH, n = 1;
  char *number = calloc(l, sizeof(char));
  int c, done = 0;

  // Already got first digit
  number[0] = (char)c1;

  do {
    c = get_next_char(json, 0);
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
  return result;
}

static void read_true(struct JSON *json,
                      int (*get_next_char)(struct JSON *, int)) {
  int c;
  for (unsigned int i = 1; i < TRUE_STRING_LENGTH; i++) {
    c = get_next_char(json, 0);
    if (c != TRUE_STRING[i]) {
      set_JSON_err(json, "Expected \"true\"");
      return;
    }
  }
}

static void read_false(struct JSON *json,
                       int (*get_next_char)(struct JSON *, int)) {
  int c;
  for (unsigned int i = 1; i < FALSE_STRING_LENGTH; i++) {
    c = get_next_char(json, 0);
    if (c != FALSE_STRING[i]) {
      set_JSON_err(json, "Expected \"false\"");
      return;
    }
  }
}

static void read_null(struct JSON *json,
                      int (*get_next_char)(struct JSON *, int)) {
  int c;
  for (unsigned int i = 1; i < NULL_STRING_LENGTH; i++) {
    c = get_next_char(json, 0);
    if (c != NULL_STRING[i]) {
      set_JSON_err(json, "Expected \"null\"");
      return;
    }
  }
}

static void read_comma_or_brace(struct JSON *json, int (*get_next_char)(struct JSON *, int)) {
  INFOF("Reading comma or brace, current char: %c", get_JSON_char(json));
  switch (get_JSON_char(json)) {
    case ',':
      get_next_char(json, 1);
      break;
    case '}':
      break;
    case ' ':
    case '\t':
    case '\n':
      get_next_char(json, 1);
      read_comma_or_brace(json, get_next_char);
      break;
    default:
      set_JSON_err(json, "Expected ',' or '}'");
  }
}

static void read_comma_or_bracket(struct JSON *json, int (*get_next_char)(struct JSON *, int)) {
  INFOF("Reading comma or bracket, current char: %c", get_JSON_char(json));
  switch (get_JSON_char(json)) {
    case ',':
      get_next_char(json, 1);
      break;
    case ']':
      break;
    case ' ':
    case '\t':
    case '\n':
      get_next_char(json, 1);
      read_comma_or_bracket(json, get_next_char);
      break;
    default:
      set_JSON_err(json, "Expected ',' or ']'");
  }
}

static struct TokenMap *parse_object(struct JSON *json,
                                     int (*get_next_char)(struct JSON *, int)) {
  INFO("Parsing object...");
  struct TokenMap *result = create_token_map();
  get_next_char(json, 1);
  while (get_JSON_char(json) != '}') {
    INFO("Reading next key-value pair");
    // Key
    if (get_JSON_char(json) != '"') {
      set_JSON_err(json, "Expected key");
      return result;
    }
    char *key = read_string(json, get_next_char);
    INFOF("Key: %s", key);

    if (get_JSON_err(json))
      return result;

    // Colon
    get_next_char(json, 1);
    if (get_JSON_char(json) != ':') {
      set_JSON_err(json, "Expected ':'");
      return result;
    }
    INFO("Read colon");
    // Value
    get_next_char(json, 1);
    struct Token *new_token = parse_next_token(json, get_next_char);

    INFO("Read value");

    if (get_JSON_err(json))
      return result;

    // Add to object
    expand_token_map(result, key, new_token);
    free(key);

    read_comma_or_brace(json, get_next_char);
    
    if (get_JSON_err(json))
      return result;
  }
  return result;
}

static struct TokenArray *parse_array(struct JSON *json, int (*get_next_char)(struct JSON *, int)) {
  INFO("Parsing array...");
  struct TokenArray *result = create_token_array();
  get_next_char(json, 1);
  while (get_JSON_char(json) != ']') {
    INFO("Reading next array token");
    struct Token *next_token = parse_next_token(json, get_next_char);

    if (get_JSON_err(json))
      return result;

    expand_token_array(result, next_token);
    read_comma_or_bracket(json, get_next_char);

    if (get_JSON_err(json))
      return result;
  }
  return result;
}

struct Token *parse_next_token(struct JSON *json,
                               int (*get_next_char)(struct JSON*, int)) {
  INFO("Parsing next token...");
  enum TokenType type = Undefined;
  union TokenValue value;
  int c = get_JSON_char(json);
  switch (c) {
  case '{':
    INFO("Processing Object...");
    type = Object;
    value.map = parse_object(json, get_next_char);
    get_next_char(json, 1);
    break;
  case '[':
    INFO("Processing Array...");
    type = Array;
    value.array = parse_array(json, get_next_char);
    get_next_char(json, 1);
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
    value.number = read_number(c, json, get_next_char);
    break;
  case '\"':
    INFO("Processing String...");
    type = String;
    value.string = read_string(json, get_next_char);
    get_next_char(json, 1);
    break;
  case TRUE_STRING_0:
    INFO("Processing Boolean (true)...");
    read_true(json, get_next_char);
    type = Boolean;
    value.boolean = 1;
    get_next_char(json, 1);
    break;
  case FALSE_STRING_0:
    INFO("Processing Boolean (false)...");
    read_false(json, get_next_char);
    type = Boolean;
    value.boolean = 0;
    get_next_char(json, 1);
    break;
  case NULL_STRING_0:
    INFO("Processing Null...");
    type = Null;
    read_null(json, get_next_char);
    get_next_char(json, 1);
    break;
  default:
    set_JSON_err(json, "Unexpected character");
    break;
  }
  struct Token *result = create_token(type, value);
  return result;
}
