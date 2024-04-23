#include "utils/JSON/parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/debug.h"

#define INITIAL_STRING_LENGTH 8
#define TRUE_STRING "true"
#define FALSE_STRING "false"
#define NULL_STRING "null"

static struct Token *parse_token_F(FILE *f, int c);
static struct Token *parse_token_S(char *str, int c, unsigned int *char_num);

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
      result = Number;
      INFO("Token: Number");
    }
    break;
  }
  return result;
}

static char *read_string_F(FILE *f) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *string = calloc(l, sizeof(char));
  int esc = 0, c, done = 0;
  do {
    c = fgetc(f);
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

static char *read_string_S(char *str, unsigned int *char_num) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *string = calloc(l, sizeof(char));
  int esc = 0, c, done = 0;
  do {
    c = str[(*char_num)++];
    if (c == EOF)
      ERR(1, "Unexpected EOF");
    // Check if we're done
    if (c == '"')
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

static int read_number_F(FILE *f) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *number = calloc(l, sizeof(char));
  int c, done = 0;
  fseek(f, -1, SEEK_CUR);
  do {
    c = fgetc(f);
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
  fseek(f, -1, SEEK_CUR);
  return result;
}

static int read_number_S(char *str, unsigned int *char_num) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *number = calloc(l, sizeof(char));
  int c, done = 0;
  (*char_num)--;
  do {
    c = str[(*char_num)++];
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
  (*char_num)--;
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

static int read_boolean_S(char *str, unsigned int *char_num) {
  int c, n = 0, done = 0, reading_true = 0;
  (*char_num)--;
  do {
    c = str[(*char_num)++];
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

static void read_null_S(char *str, unsigned int *char_num) {
  int c;
  (*char_num)--;
  for (int i = 0; i < 4; i++) {
    c = str[(*char_num)++];
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

static int skip_whitespaces_S(char *str, unsigned int *char_num) {
  int c;
  do {
    c = str[(*char_num)++];
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

static int skip_to_next_object_token_S(char *str, unsigned int *char_num) {
  int c = skip_whitespaces_S(str, char_num);
  // Comma or '}'
  switch (c) {
  case ',':
    // Must be '"'
    c = skip_whitespaces_S(str, char_num);
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

static struct TokenMap *parse_object_S(char *str, unsigned int *char_num) {
  struct TokenMap *result = create_token_map();
  int c = skip_whitespaces_S(str, char_num);
  while (c != '}') {
    // Key
    if (c != '"')
      ERRF(1, "Expected key, got [%c]", c);
    char *key = read_string_S(str, char_num);
    c = skip_whitespaces_S(str, char_num);
    // Colon
    if (c != ':')
      ERRF(1, "Expected ':', got [%c]", c);
    c = skip_whitespaces_S(str, char_num);
    // Value
    struct Token *new_token = parse_token_S(str, c, char_num);
    // Add to object
    expand_token_map(result, key, new_token);
    free(key);
    c = skip_to_next_object_token_S(str, char_num);
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

static int skip_to_next_array_token_S(char *str, unsigned int *char_num) {
  int c = skip_whitespaces_S(str, char_num);
  // Comma or ']'
  switch (c) {
  case ',':
    // Must not be ',' or ']'
    c = skip_whitespaces_S(str, char_num);
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

static struct TokenMap *parse_array_S(char *str, unsigned int *char_num) {
  struct TokenMap *result = create_token_map();
  int c = skip_whitespaces_S(str, char_num);
  while (c != ']') {
    struct Token *next_token = parse_token_S(str, c, char_num);
    expandTokenMapN(result, next_token);
    c = skip_to_next_array_token_S(str, char_num);
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

static struct Token *parse_token_S(char *str, int c, unsigned int *char_num) {
  union TokenValue value;
  enum TokenType type = identify_token(c);
  switch (type) {
  case Undefined:
    ERRF(1, "Unexpected character: %i [%c]", c, c);
    break;
  case Object:
    INFO("Processing Object...");
    value.map = parse_object_S(str, char_num);
    break;
  case Array:
    INFO("Processing Array...");
    value.map = parse_array_S(str, char_num);
    break;
  case Number:
    INFO("Processing Number...");
    value.number = read_number_S(str, char_num);
    break;
  case String:
    INFO("Processing String...");
    value.string = read_string_S(str, char_num);
    break;
  case Boolean:
    INFO("Processing Boolean...");
    value.boolean = read_boolean_S(str, char_num);
    break;
  case Null:
    INFO("Processing Null...");
    read_null_S(str, char_num);
    break;
  }
  struct Token *result = create_token(type, value);
  return result;
}

struct Token *read_json_file(char *filename) {
  FILE *f;
  f = fopen(filename, "r");
  if (!f)
    ERRF(1, "Unable to open file: %s", filename);
  int c = skip_whitespaces_F(f);
  struct Token *result = parse_token_F(f, c);
  fclose(f);
  return result;
}

struct Token *read_json_string(char *str) {
  unsigned int n = 0;
  int c = skip_whitespaces_S(str, &n);
  struct Token *result = parse_token_S(str, c, &n);
  return result;
}
