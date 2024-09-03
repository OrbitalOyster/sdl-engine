#include <stdio.h>
#include <string.h>

#include "config.h"
#include "core.h"
#include "input.h"
#include "utils/JSON/JSON.h"
#include "utils/debug.h"

#include "gui/gui.h"

int quit = 0;

void on_key_down(SDL_Scancode key) {
  INFOF("Key pressed: %c [%u]", SDL_GetKeyFromScancode(key), key);
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
  INFOF("Key released: %c [%u]", SDL_GetKeyFromScancode(key), key);
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
  struct Core *core = create_core(config->window_width, config->window_height,
                                  config->window_title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return 1;
  }

  struct GUI *gui = create_gui(core);
  struct GUI_Container *c = calloc(1, sizeof(struct GUI_Container));
  *c = (struct GUI_Container) {
    .top_distance = {.type = GUT_AUTO, .px = 10},
    .top_anchor = {.type = GUT_ABSOLUTE, .px = 0},

    .right_distance = {.type = GUT_ABSOLUTE, .px = 10},
    .right_anchor = {.type = GUT_ABSOLUTE, .px = 0},

    .bottom_distance = {.type = GUT_ABSOLUTE, .px = 10},
    .bottom_anchor = {.type = GUT_ABSOLUTE, .px = 0},

    .left_distance = {.type = GUT_AUTO, .px = 10},
    .left_anchor = {.type = GUT_ABSOLUTE, .px = 0},

    .width = {.type = GUT_ABSOLUTE, .px = 250},
    .height = {.type = GUT_ABSOLUTE, .px = 250}
  };
  add_gui_container(gui, c);
  INFO2F("GUI: %u", gui->number_of_containers);

  // Init input
  reset_key_input();
  register_on_key_down_func(on_key_down);
  register_on_key_up_func(on_key_up);
  // Renderer
  SDL_Renderer *renderer = get_renderer(core);
  // Main cycle
  while (!quit) {
    // Input
    process_input(&quit);
    // Render
    SDL_SetRenderDrawColor(renderer, 0x88, 0x88, 0xCC, 0xFF);
    SDL_RenderClear(renderer);

    // GUI
    render_gui(gui);

    // Done
    SDL_RenderPresent(renderer);
    // Delay
    SDL_Delay(50);
  }
  // Cleanup
  destroy_core(core);
  destroy_config(config);
}
