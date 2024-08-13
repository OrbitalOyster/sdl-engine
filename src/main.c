#include <stdio.h>
#include <string.h>

#include "core.h"
#include "input.h"
#include "utils/JSON/JSON.h"
#include "utils/debug.h"

struct Config {
  int32_t window_width;
  int32_t window_height;
  char *window_title;
};

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

int quit = 0;

void on_key_down(SDL_Scancode key) {
  INFOF("Key pressed: %c [%i]", SDL_GetKeyFromScancode(key), key);
  switch (key) {
  case 20: // q
    quit = 1;
    break;
  case 26: // w
    break;
  case 7: // d
    break;
  case 22: // s
    break;
  case 4: // a
    break;
  // Debug section
  case 62: // F5
    break;
  case 63: // F6
    break;
  default:
    break;
  }
}

void on_key_up(SDL_Scancode key) {
  INFOF("Key released: %c [%i]", SDL_GetKeyFromScancode(key), key);
  switch (key) {
  case 26: // w
    break;
  case 7: // d
    break;
  case 22: // s
    break;
  case 4: // a
    break;
  default:
    break;
  }
}

int main() {
  // Load config
  struct Config *config = load_config("config.json");
  // Init core
  Core *core = init_core(config->window_width, config->window_height,
                         config->window_title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return 1;
  }

  reset_key_input();
  register_on_key_down_func(on_key_down);
  register_on_key_up_func(on_key_up);

  SDL_Renderer *renderer = get_renderer(core);
  while (!quit) {
    // Input
    process_input(&quit);
    // Render
    SDL_SetRenderDrawColor(renderer, 0x88, 0x88, 0xCC, 0xFF);
    SDL_RenderClear(renderer);
    // Done
    SDL_RenderPresent(renderer);
    // Delay
    SDL_Delay(50);
  }
  // Cleanup
  destroy_core(core);
  destroy_config(config);
}
