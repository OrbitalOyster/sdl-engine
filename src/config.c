#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "utils/JSON/parser.h"
#include "utils/debug.h"

struct Config *load_config(char *filename) {
  struct JSON_Parser *parser = create_JSON_parser();
  struct Token *config_json = parse_JSON_file(parser, filename);
  // Check if config is parsed ok
  char *parser_err = get_JSON_parser_err(parser);
  if (parser_err)
    ERRF(1, "Failed to parse config: %s", parser_err);

  struct Config *result = calloc(1, sizeof(struct Config));
  result->window_width = read_token_number(config_json, "window/width");
  result->window_height = read_token_number(config_json, "window/height");
  char *title = rea_token_string(config_json, "window/title");

  char *err = get_token_err(config_json);
  if (err) {
    WARNF("Invalid config: %s", err);
    destroy_token(config_json);
    return NULL;
  }

  result->window_title = calloc(strlen(title) + 1, sizeof(char));
  strcpy(result->window_title, title);

  // Cleanup
  destroy_token(config_json);
  destroy_JSON_parser(parser);

  return result;
}

void destroy_config(struct Config *config) {
  free(config->window_title);
  free(config);
}
