#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "utils/JSON/parser.h"
#include "utils/debug.h"

struct Config *load_config(char *filename) {
  struct Token *config_json = parse_JSON_file(filename);
  // Check if config is parsed ok
  // if (get_JSON_err(config_json))
  //  ERR(1, get_JSON_err(config_json));
  // Check window props
  //  check_JSON_token(config_json, Object, "");
  //  check_JSON_token(config_json, Number, "window/width");
  //  check_JSON_token(config_json, Number, "window/height");
  // check_JSON_token(config_json, String, "window/title");
  //  check_JSON_token(config_json, String, "window/%s", "title");
  // if (get_JSON_err(config_json))
  //  ERRF(1, "Invalid config: %s", get_JSON_err(config_json));
  // Create and read config
  struct Config *result = calloc(1, sizeof(struct Config));
  result->window_width = read_token_number(config_json, "window/width");
  result->window_height = read_token_number(config_json, "window/height");
  char *title = rea_token_string(config_json, "window/title");

  /*
  char *err = get_JSON_err(config_json);
  if (err) {
    WARNF("Invalid config: %s", err);
    destroy_JSON(config_json);
    return NULL;
  }
  */

  result->window_title = calloc(strlen(title) + 1, sizeof(char));
  strcpy(result->window_title, title);
  // destroy_JSON(config_json);
  return result;
}

void destroy_config(struct Config *config) {
  free(config->window_title);
  free(config);
}
