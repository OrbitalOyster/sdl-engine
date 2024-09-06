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
  struct GUI_Container *c0 = calloc(1, sizeof(struct GUI_Container));
  struct GUI_Container *c1 = calloc(1, sizeof(struct GUI_Container));
  struct GUI_Container *c2 = calloc(1, sizeof(struct GUI_Container));

  /* Centered (1/9) */
  *c0 = (struct GUI_Container){.top = {.type = GUT_RELATIVE,
                                                .pct = .5,
                                                .ref_type = GUT_RELATIVE,
                                                .ref_pct = .5},
                               .left = {.type = GUT_RELATIVE,
                                                 .pct = .5,
                                                 .ref_type = GUT_RELATIVE,
                                                 .ref_pct = .5},
                               .width = {.type = GUT_RELATIVE, .pct = .333},
                               .height = {.type = GUT_RELATIVE, .pct = .333}};

  /* Top banner */
  *c1 = (struct GUI_Container){
      .top = {.type = GUT_ABSOLUTE, .px = 5, .ref_type = GUT_ABSOLUTE},
      .right = {.type = GUT_ABSOLUTE,
                         .px = 5,
                         .ref_type = GUT_ABSOLUTE},
      .left = {.type = GUT_ABSOLUTE,
                        .px = 5,
                        .ref_type = GUT_ABSOLUTE},
      .height = {.type = GUT_ABSOLUTE, .px = 150}};

  /* Bottom right */
  *c2 = (struct GUI_Container){.right = {.type = GUT_ABSOLUTE,
                                                  .px = 25,
                                                  .ref_type = GUT_ABSOLUTE},
                               .bottom = {.type = GUT_ABSOLUTE,
                                                   .px = 25,
                                                   .ref_type = GUT_ABSOLUTE},
                               .width = {.type = GUT_ABSOLUTE, .px = 250},
                               .height = {.type = GUT_ABSOLUTE, .px = 250}};

  add_gui_container(gui, c0);
  add_gui_container(gui, c1);
  add_gui_container(gui, c2);

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
