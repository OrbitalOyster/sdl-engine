#include "utils/JSON/JSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/parser.h"
#include "utils/JSON/stringify.h"
#include "utils/JSON/token.h"
#include "utils/debug.h"

#define MAX_JSON_ERR_LENGTH 64

struct JSON {
  struct Token *root;
  size_t char_num;
  size_t line_num;
  size_t col_num;
  char *err;
  void *source;
};

struct JSON *create_JSON() {
  struct JSON *json = calloc(1, sizeof(struct JSON));
  *json = (struct JSON){.root = NULL,
                        .char_num = 1,
                        .line_num = 1,
                        .col_num = 1,
                        .err = NULL,
                        .source = NULL};
  return json;
}

void set_JSON_err(struct JSON *json, char *err) {
  if (json->err)
    free(json->err);

  json->err = calloc(strlen(err) + 1, sizeof(char));
  strcpy(json->err, err);
}

char *get_JSON_err(struct JSON *json) {
  return json->err;
}

static int get_next_char_F(void *json_arg) {

  struct JSON *json = (struct JSON *)json_arg;
  FILE *f = (FILE *)json->source;

  int c = getc(f);
  INFOF("Next char %i [%c]", c, c);
  json->char_num++;
  if (c == '\n') {
    json->line_num++;
    json->col_num = 1;
  } else
    json->col_num++;
  return c;
}

void rewind_F(void *json_arg) {

  struct JSON *json = (struct JSON *)json_arg;
  FILE *f = (FILE *)json->source;

  INFO("Rewind...");
  fseek(f, -1, SEEK_CUR);
  json->char_num--;
}

struct JSON *file_to_JSON(char *filename) {
  struct JSON *json = create_JSON();
  FILE *f;

  // Try to open file
  f = fopen(filename, "r");
  if (!f) {
    set_JSON_err(json, "Unable to open file");
    return json;
  }
  json->source = f;
  // Skip trailing whitespaces
  struct Token *root = parse_next_token(json, get_next_char_F, rewind_F);

  if (get_JSON_err(json))
    INFO2F("JSON error: %s", get_JSON_err(json));

  json->root = root;
  fclose(f);

  return json;
}

char *JSON_to_string(struct JSON *json) {
  return token_to_string(json->root);
}

void destroy_JSON(struct JSON *json) {
  destroy_token(json->root);
  free(json);
}
