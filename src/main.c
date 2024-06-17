#include <stdio.h>

#include "core.h"
#include "input.h"
#include "utils/JSON/JSON.h"
#include "utils/debug.h"

struct Config {
  int32_t width;
  int32_t height;
  char *title;
};

struct Config *loadConfig(char *filename) {
  struct JSON *config_json = file_to_JSON(filename);
  // Check if config is parsed ok
  if (get_JSON_err(config_json))
    ERR(1, get_JSON_err(config_json));
  // Check if root token is object
  if (!JSON_is_object(config_json))
    ERR(1, "Invalid config: not an object");
  // Check if "width" and "height" props are present
  if (!JSON_object_has_prop(config_json, "windowWidth") ||
      !JSON_object_has_prop(config_json, "windowHeight"))
    ERR(1, "Invalid config: missing screen size props");

  struct Config *result = calloc(1, sizeof(struct Config));
  destroy_JSON(config_json);
  return result;
}

int main() {
  int quit = 0;

  struct Config *config = loadConfig("config.json");

  if (config) {
  }

  Core *core = init_core(640, 480, "Untitled");
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return 0;
  }

  reset_key_input();

  SDL_Renderer *renderer = get_renderer(core);
  while (!quit) {
    // Input
    process_input(&quit);
    // Render
    SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(renderer);
    // Done
    SDL_RenderPresent(renderer);
    // Delay
    SDL_Delay(50);
  }

  destroy_core(core);
}
