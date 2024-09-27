#include <stdio.h>
#include <string.h>

#include "config.h"
#include "core.h"
#include "gui/gui.h"
#include "input.h"
#include "utils/debug.h"

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
  if (!config) {
    ERR(1, "Failed to load config");
  }

  // Init core
  struct Core *core = create_core(config->window_width, config->window_height,
                                  config->window_title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return 1;
  }

  struct GUI *gui = create_gui(core, "assets/gui/skin.json");
  /*struct GUI_Container *c0 = calloc(1, sizeof(struct GUI_Container));*/
  struct GUI_Container *c1 = calloc(1, sizeof(struct GUI_Container));
  struct GUI_Container *c2 = calloc(1, sizeof(struct GUI_Container));

  /* Centered (1/9) */
  /**c0 = (struct GUI_Container){.top = {.type = GUT_RELATIVE, .f = .5},*/
  /*                             .top_p = {.type = GUT_RELATIVE, .f = .5},*/
  /*                             .left = {.type = GUT_RELATIVE, .f = .5},*/
  /*                             .left_p = {.type = GUT_RELATIVE, .f = .5},*/
  /*                             .width = {.type = GUT_RELATIVE, .f = .33},*/
  /*                             .height = {.type = GUT_RELATIVE, .f = .33}};*/

  /* Bottom right */
  *c2 = (struct GUI_Container){.right = {.type = GUT_ABSOLUTE},
                               .right_p = {.type = GUT_ABSOLUTE, .px = -25},
                               .bottom = {.type = GUT_ABSOLUTE},
                               .bottom_p = {.type = GUT_ABSOLUTE, .px = -25},
                               .width = {.type = GUT_ABSOLUTE, .px = 100},
                               .height = {.type = GUT_ABSOLUTE, .px = 50}};
  struct GUI_Button b2 = (struct GUI_Button){.container = c2};

  /* Top banner */
  *c1 = (struct GUI_Container){.top = {.type = GUT_ABSOLUTE, .px = 5},
                               .right = {.type = GUT_ABSOLUTE, .px = 5},
                               .left = {.type = GUT_ABSOLUTE, .px = 5},
                               .height = {.type = GUT_ABSOLUTE, .px = 150},
                               .number_of_containers = 1,
                               .containers =
                                   calloc(1, sizeof(struct GUI_Container *))};
  c1->containers[0] = b2.container;
  struct GUI_Window w1 = (struct GUI_Window){.container = c1};

  /*add_gui_container(gui, c0);*/
  add_gui_window(gui, &w1);
  /*add_gui_button(gui, &b2);*/

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
