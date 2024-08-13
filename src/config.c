#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "utils/JSON/JSON.h"
#include "utils/debug.h"

struct Config *load_config(char *filename) {
  struct JSON *config_json = file_to_JSON(filename);
  // Check if config is parsed ok
  if (get_JSON_err(config_json))
    ERR(1, get_JSON_err(config_json));
  // Check window props
  check_JSON_token(config_json, "window/width", Number);
  check_JSON_token(config_json, "window/height", Number);
  check_JSON_token(config_json, "window/title", String);
  if (get_JSON_err(config_json))
    ERRF(1, "Invalid config: %s", get_JSON_err(config_json));
  // Create and read config
  struct Config *result = calloc(1, sizeof(struct Config));
  result->window_width = JSON_get_number_prop(config_json, "window/width");
  result->window_height = JSON_get_number_prop(config_json, "window/height");
  char *title = JSON_get_string_prop(config_json, "window/title");
  result->window_title = calloc(strlen(title) + 1, sizeof(char));
  strcpy(result->window_title, title);
  destroy_JSON(config_json);
  return result;
}

void destroy_config(struct Config *config) {
  free(config->window_title);
  free(config);
}
