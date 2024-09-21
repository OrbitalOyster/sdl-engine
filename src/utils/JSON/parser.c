#include "utils/JSON/parser.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "utils/JSON/token-array.h"
#include "utils/JSON/token-map.h"
#include "utils/debug.h"

#define MAX_PARSER_ERR_LENGTH (255 + 1)

#define CHUNK_LENGTH 4096

#define TRUE_STRING "true"
#define TRUE_STRING_0 't'
#define TRUE_STRING_LENGTH 4

#define FALSE_STRING "false"
#define FALSE_STRING_0 'f'
#define FALSE_STRING_LENGTH 5

#define NULL_STRING "null"
#define NULL_STRING_0 'n'
#define NULL_STRING_LENGTH 4

#define INITIAL_STRING_LENGTH 64

struct JSON_Parser {
  int c;
  size_t char_num;
  size_t line_num;
  size_t col_num;
  char *err;
  void *source;
  char *chunk;
  size_t chunk_read;
  char *number_str;
};

static struct Token *parse_next_token(struct JSON_Parser *parser,
                                      int (*get_next_char)(struct JSON_Parser *,
                                                           int));

struct JSON_Parser *create_JSON_parser() {
  struct JSON_Parser *parser = calloc(1, sizeof(struct JSON_Parser));
  *parser = (struct JSON_Parser){
      .c = '\n',
      .char_num = 0,
      .line_num = 0,
      .col_num = 0,
      .err = NULL,
      .source = NULL,
      .chunk = calloc(CHUNK_LENGTH, sizeof(char)),
      .chunk_read = 0,
      .number_str = calloc(MAX_NUMBER_STR_LENGTH, sizeof(char))};
  return parser;
}

static int is_whitespace(int c) { return c == ' ' || c == '\t' || c == '\n'; }
static int is_digit(int c) { return (c >= '0' && c <= '9'); }

static void set_parser_err(struct JSON_Parser *parser, char *err, ...) {
  // Magic
  char *err_s = calloc(MAX_PARSER_ERR_LENGTH, sizeof(char));
  va_list args;
  va_start(args, err);
  int chars = vsnprintf(err_s, MAX_PARSER_ERR_LENGTH, err, args);
  if (chars >= MAX_PARSER_ERR_LENGTH)
    WARNF("Parser error too large (%i)", chars);
  va_end(args);
  // Copy to err
  parser->err = calloc(strlen(err_s) + 1, sizeof(char));
  strcpy(parser->err, err_s);
}

static int get_next_char_F(struct JSON_Parser *parser, int skip_whitespaces) {
  FILE *f = (FILE *)parser->source;
  do {
    parser->char_num++;
    // Check previous char
    if (parser->c == '\n') {
      parser->line_num++;
      parser->col_num = 1;
    } else
      parser->col_num++;

    parser->c = parser->chunk[parser->chunk_read++];
    if (parser->chunk_read == CHUNK_LENGTH) {
      INFO("Reading next chunk...");
      const size_t fread_res =
          fread(parser->chunk, sizeof(char), CHUNK_LENGTH, f);
      if (fread_res != CHUNK_LENGTH && ferror(f))
        set_parser_err(parser, "Error reading file");
      parser->chunk_read = 0;
    }
    INFOF("Next char %i [%c]", parser->c, parser->c);
  } while (skip_whitespaces && is_whitespace(parser->c));

  return parser->c;
}

static int get_next_char_S(struct JSON_Parser *parser, int skip_whitespaces) {
  char *s = (char *)parser->source;

  do {
    parser->char_num++;
    // Check previous char
    if (parser->c == '\n') {
      parser->line_num++;
      parser->col_num = 1;
    } else
      parser->col_num++;

    parser->c = s[parser->char_num - 1];
    INFOF("Next char %i [%c]", parser->c, parser->c);
  } while (skip_whitespaces && is_whitespace(parser->c));

  return parser->c;
}

static char *read_string(struct JSON_Parser *parser,
                         int (*get_next_char)(struct JSON_Parser *, int)) {
  size_t l = INITIAL_STRING_LENGTH, n = 0;
  char *string = calloc(l, sizeof(char));
  int esc = 0, c, done = 0;
  do {
    c = get_next_char(parser, 0);
    switch (c) {
    // No unexpected EOF
    case EOF:
      set_parser_err(parser, "Unexpected EOF");
      free(string);
      return NULL;
    // No multiline strings
    case '\n':
      set_parser_err(parser, "Unexpected \\n");
      free(string);
      return NULL;
    // Check if we're done
    case '"':
      done = !esc; // Ignore [\"], finish parsing on ["]
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
  if (parser->err)
    WARNF("JSON error: %s, line: %lu, col: %lu, char: %i [%c]", parser->err,
          parser->line_num, parser->col_num, parser->c, parser->c);
  INFOF("Read string: %s", string);
  return string;
}

static int read_number(struct JSON_Parser *parser,
                       int (*get_next_char)(struct JSON_Parser *, int)) {
  unsigned int n = 0;
  char *number = parser->number_str;

  while (is_digit(parser->c)) {
    number[n++] = (char)parser->c;
    get_next_char(parser, 0);
  }

  number[n] = '\0';
  INFOF("Read Number: %s", number);
  int result = atoi(number);
  INFOF("Actual number: %s", number);
  return result;
}

static void read_true(struct JSON_Parser *parser,
                      int (*get_next_char)(struct JSON_Parser *, int)) {
  for (int i = 1; i < TRUE_STRING_LENGTH; i++)
    if (get_next_char(parser, 0) != TRUE_STRING[i]) {
      set_parser_err(parser, "Expected \"true\"");
      return;
    }
}

static void read_false(struct JSON_Parser *parser,
                       int (*get_next_char)(struct JSON_Parser *, int)) {
  for (int i = 1; i < FALSE_STRING_LENGTH; i++)
    if (get_next_char(parser, 0) != FALSE_STRING[i]) {
      set_parser_err(parser, "Expected \"false\"");
      return;
    }
}

static void read_null(struct JSON_Parser *parser,
                      int (*get_next_char)(struct JSON_Parser *, int)) {
  for (int i = 1; i < NULL_STRING_LENGTH; i++)
    if (get_next_char(parser, 0) != NULL_STRING[i]) {
      set_parser_err(parser, "Expected \"null\"");
      return;
    }
}

static void read_comma_or_brace(struct JSON_Parser *parser,
                                int (*get_next_char)(struct JSON_Parser *,
                                                     int)) {
  INFOF("Reading comma or brace, current char: %c", parser->c);
  switch (parser->c) {
  case ',':
    get_next_char(parser, 1);
    break;
  case '}':
    break;
  case ' ':
  case '\t':
  case '\n':
    get_next_char(parser, 1);
    read_comma_or_brace(parser, get_next_char);
    break;
  default:
    set_parser_err(parser, "Expected ',' or '}'");
  }
}

static void read_comma_or_bracket(struct JSON_Parser *parser,
                                  int (*get_next_char)(struct JSON_Parser *,
                                                       int)) {
  INFOF("Reading comma or bracket, current char: %c", parser->c);
  switch (parser->c) {
  case ',':
    get_next_char(parser, 1);
    break;
  case ']':
    break;
  case ' ':
  case '\t':
  case '\n':
    get_next_char(parser, 1);
    read_comma_or_bracket(parser, get_next_char);
    break;
  default:
    set_parser_err(parser, "Expected ',' or ']'");
  }
}

static struct TokenMap *parse_object(struct JSON_Parser *parser,
                                     int (*get_next_char)(struct JSON_Parser *,
                                                          int)) {
  INFO("Parsing object...");
  struct TokenMap *result = create_token_map();
  get_next_char(parser, 1);
  while (parser->c != '}') {
    INFO("Reading next key-value pair");
    // Key
    if (parser->c != '"') {
      set_parser_err(parser, "Expected key");
      return result;
    }
    char *key = read_string(parser, get_next_char);
    INFOF("Key: %s", key);

    if (parser->err)
      return result;

    // Colon
    if (get_next_char(parser, 1) != ':') {
      set_parser_err(parser, "Expected ':'");
      return result;
    }
    // Value
    get_next_char(parser, 1);
    struct Token *new_token = parse_next_token(parser, get_next_char);

    if (parser->err)
      return result;

    // Add to object
    expand_token_map(result, key, new_token);
    free(key);

    read_comma_or_brace(parser, get_next_char);

    if (parser->err)
      return result;
  }
  return result;
}

static struct TokenArray *parse_array(struct JSON_Parser *parser,
                                      int (*get_next_char)(struct JSON_Parser *,
                                                           int)) {
  INFO("Parsing array...");
  struct TokenArray *result = create_token_array();
  get_next_char(parser, 1);
  while (parser->c != ']') {
    INFO("Reading next array token");
    struct Token *next_token = parse_next_token(parser, get_next_char);

    if (parser->err)
      return result;

    expand_token_array(result, next_token);
    read_comma_or_bracket(parser, get_next_char);

    if (parser->err)
      return result;
  }
  return result;
}

static struct Token *parse_next_token(struct JSON_Parser *parser,
                                      int (*get_next_char)(struct JSON_Parser *,
                                                           int)) {
  INFO("Parsing next token...");
  enum TokenType type = Undefined;
  union TokenValue value;
  switch (parser->c) {
  case '{':
    INFO("Processing Object...");
    type = Object;
    value.map = parse_object(parser, get_next_char);
    get_next_char(parser, 1);
    break;
  case '[':
    INFO("Processing Array...");
    type = Array;
    value.array = parse_array(parser, get_next_char);
    get_next_char(parser, 1);
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
    value.number = read_number(parser, get_next_char);
    break;
  case '\"':
    INFO("Processing String...");
    type = String;
    value.string = read_string(parser, get_next_char);
    get_next_char(parser, 1);
    break;
  case TRUE_STRING_0:
    INFO("Processing Boolean (true)...");
    read_true(parser, get_next_char);
    type = Boolean;
    value.boolean = 1;
    get_next_char(parser, 1);
    break;
  case FALSE_STRING_0:
    INFO("Processing Boolean (false)...");
    read_false(parser, get_next_char);
    type = Boolean;
    value.boolean = 0;
    get_next_char(parser, 1);
    break;
  case NULL_STRING_0:
    INFO("Processing Null...");
    type = Null;
    read_null(parser, get_next_char);
    get_next_char(parser, 1);
    break;
  default:
    set_parser_err(parser, "Unexpected character");
    break;
  }
  struct Token *result = create_token(type, value);
  return result;
}

struct Token *parse_JSON_file(struct JSON_Parser *parser, char *filename) {

  // Try to open file
  FILE *f = fopen(filename, "r");
  if (!f) {
    set_parser_err(parser, "Unable to open file \"%s\"", filename);
    return NULL;
  }
  parser->source = f;

  const size_t fread_res = fread(parser->chunk, sizeof(char), CHUNK_LENGTH, f);
  if (fread_res != CHUNK_LENGTH && ferror(f))
    set_parser_err(parser, "Error reading file \"%s\"", filename);

  // Skip trailing whitespaces
  get_next_char_F(parser, 1);
  struct Token *result = parse_next_token(parser, get_next_char_F);

  if (parser->err)
    WARNF("JSON error: %s, line: %lu, col: %lu, char: %i [%c]", parser->err,
          parser->line_num, parser->col_num, parser->c, parser->c);

  fclose(f);

  return result;
}

struct Token *parse_JSON_string(struct JSON_Parser *parser, char *s) {
  parser->source = (void *)s;

  // Skip trailing whitespaces
  get_next_char_S(parser, 1);
  struct Token *result = parse_next_token(parser, get_next_char_S);

  if (parser->err)
    WARNF("JSON error: %s, line: %lu, col: %lu, char: %i [%c]", parser->err,
          parser->line_num, parser->col_num, parser->c, parser->c);

  return result;
}

char *get_JSON_parser_err(struct JSON_Parser *parser) { return parser->err; }

void destroy_JSON_parser(struct JSON_Parser *parser) {
  free(parser->err);
  // free(parser->source);
  free(parser->chunk);
  free(parser->number_str);
  free(parser);
}
