#include "utils/JSON/JSON.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/JSON/parser.h"
#include "utils/JSON/stringify.h"
#include "utils/JSON/token.h"
#include "utils/debug.h"

#define MAX_JSON_ERR_LENGTH 64
#define CHUNK_LENGTH 1024

struct JSON {
  struct Token *root;
  int c;
  int skip;
  size_t char_num;
  size_t line_num;
  size_t col_num;
  char *err;
  void *source;
  char *chunk;
  size_t chunk_read;
};

struct JSON *create_JSON() {
  struct JSON *json = calloc(1, sizeof(struct JSON));
  *json = (struct JSON){.root = NULL,
                        .c = '\n',
                        .skip = 0,
                        .char_num = 0,
                        .line_num = 0,
                        .col_num = 0,
                        .err = NULL,
                        .source = NULL,
                        .chunk = calloc(CHUNK_LENGTH, sizeof(char)),
                        .chunk_read = 0};
  return json;
}

void set_JSON_err(struct JSON *json, char *err) {
  if (json->err)
    free(json->err);

  json->err = calloc(strlen(err) + 1, sizeof(char));
  strcpy(json->err, err);
}

char *get_JSON_err(struct JSON *json) { return json->err; }

static int get_next_char_F(struct JSON *json) {

  if (json->skip) {
    json->skip = 0;
    return json->c;
  }

  FILE *f = (FILE *)json->source;

  json->char_num++;
  // Check previous char
  if (json->c == '\n') {
    json->line_num++;
    json->col_num = 1;
  } else
    json->col_num++;

  json->c = json->chunk[json->chunk_read++];
  if (json->chunk_read == CHUNK_LENGTH) {
    INFO("Reading next chunk...");
    const size_t fread_res = fread(json->chunk, sizeof(char), CHUNK_LENGTH, f);
    if (fread_res != CHUNK_LENGTH && ferror(f))
      set_JSON_err(json, "Error reading file");
    json->chunk_read = 0;
  }

  INFOF("Next char %i [%c]", json->c, json->c);
  return json->c;
}

void skip_next_JSON_char(struct JSON *json) {
  json->skip = 1;
}

static int get_next_char_S(struct JSON *json) {

  if (json->skip) {
    json->skip = 0;
    return json->c;
  }

  char *s = (char *)json->source;

  json->char_num++;
  // Check previous char
  if (json->c == '\n') {
    json->line_num++;
    json->col_num = 1;
  } else
    json->col_num++;

  json->c = s[json->char_num - 1];
  INFOF("Next char %i [%c]", json->c, json->c);
  return json->c;
}

struct JSON *file_to_JSON(char *filename) {
  struct JSON *json = create_JSON();

  // Try to open file
  FILE *f = fopen(filename, "r");
  if (!f) {
    set_JSON_err(json, "Unable to open file");
    return json;
  }
  json->source = f;

  const size_t fread_res = fread(json->chunk, sizeof(char), CHUNK_LENGTH, f);
  if (fread_res != CHUNK_LENGTH && ferror(f))
    set_JSON_err(json, "Error reading file");

  // Skip trailing whitespaces
  struct Token *root = parse_next_token(json, get_next_char_F);

  if (get_JSON_err(json))
    INFO2F("JSON error: %s, line: %lu, col: %lu, char: %i [%c]",
           get_JSON_err(json), json->line_num, json->col_num, json->c, json->c);

  json->root = root;
  fclose(f);

  return json;
}

struct JSON *string_to_JSON(char *s) {
  struct JSON *json = create_JSON();
  json->source = (void *)s;
  // Skip trailing whitespaces
  struct Token *root = parse_next_token(json, get_next_char_S);

  if (get_JSON_err(json))
    INFO2F("JSON error: %s, line: %lu, col: %lu, char: %i [%c]",
           get_JSON_err(json), json->line_num, json->col_num, json->c, json->c);

  json->root = root;
  return json;
}

char *JSON_to_string(struct JSON *json) { return token_to_string(json->root); }

void destroy_JSON(struct JSON *json) {
  destroy_token(json->root);
  free(json->chunk);
  free(json);
}
