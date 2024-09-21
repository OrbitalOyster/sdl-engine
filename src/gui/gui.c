#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include "gui/container_foo.h"
#include "png.h"
#include "utils/JSON/parser.h"
#include "utils/JSON/token-array.h"
#include "utils/debug.h"

#define MAX_GUI_CONTAINERS 255

struct GUI {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

struct GUI_Skin {
  SDL_Texture *texture;
  unsigned int atlas_size;
  struct Container_Foo *atlas;
};

struct GUI_Skin *load_gui_skin(char *filename) {
  struct GUI_Skin *result = calloc(1, sizeof(struct GUI_Skin));
  struct JSON_Parser *parser = create_JSON_parser();
  struct Token *json = parse_JSON_file(parser, filename);
  //  check_JSON_token(json, String, "filename");
  //  check_JSON_token(json, Array, "atlas");
  //  if (get_JSON_err(json)) {
  //    WARNF("JSON err: %s", get_JSON_err(json));
  //    return NULL;
  //  }
  INFO2F("N: %i", read_token_number(json, "atlas/0/bottomLeft/1"));

  //  struct TokenArray *atlas = get_JSON_get_array_token(json, "atlas");
  //  INFO2F("Size: %lu", get_token_array_size(atlas));

  int n = read_token_number(json, "foo/%i", 2);
  // if (get_JSON_err(json)) {
  //   WARNF("JSON err: %s", get_JSON_err(json));
  // }
  INFO2F("Token: %i", n);

  result->atlas_size = 0;
  return result;
}

struct GUI *create_gui(struct Core *core, char *png_filename) {
  struct GUI *result = calloc(1, sizeof(struct GUI));
  result->window = get_window(core);
  result->renderer = get_renderer(core);

  load_gui_skin("assets/gui/skin.json");

  result->texture = load_png(get_renderer(core), png_filename);
  result->number_of_containers = 0;
  result->containers =
      calloc(MAX_GUI_CONTAINERS, sizeof(struct GUI_Container *));
  return result;
}

void add_gui_container(struct GUI *gui, struct GUI_Container *container) {
  gui->containers[gui->number_of_containers++] = container;
}

void render_gui(struct GUI *gui) {
  SDL_Window *window = gui->window;
  SDL_Renderer *renderer = gui->renderer;

  int root_width, root_height;
  SDL_GetWindowSize(window, &root_width, &root_height);

  struct Container_Foo foo = (struct Container_Foo){
      .center = (SDL_Rect){.x = 32, .y = 32, .w = 16, .h = 16},
      .top = (SDL_Rect){.x = 32, .y = 16, .w = 16, .h = 16},
      .right = (SDL_Rect){.x = 48, .y = 32, .w = 16, .h = 16},
      .bottom = (SDL_Rect){.x = 32, .y = 48, .w = 16, .h = 16},
      .left = (SDL_Rect){.x = 16, .y = 32, .w = 16, .h = 16},
      .top_left = (SDL_Rect){.x = 16, .y = 16, .w = 16, .h = 16},
      .top_right = (SDL_Rect){.x = 48, .y = 16, .w = 16, .h = 16},
      .bottom_right = (SDL_Rect){.x = 48, .y = 48, .w = 16, .h = 16},
      .bottom_left = (SDL_Rect){.x = 16, .y = 48, .w = 16, .h = 16}};

  for (unsigned int i = 0; i < gui->number_of_containers; i++)
    render_container(renderer, gui->containers[i], root_width, root_height,
                     gui->texture, &foo);
}
