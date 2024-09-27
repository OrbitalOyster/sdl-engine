#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include "png.h"
#include "stretchable.h"
#include "JSON/parser.h"
#include "JSON/token.h"

#include "utils/debug.h"

#define MAX_GUI_CONTAINERS 255

struct GUI_Skin {
  SDL_Texture *texture;
  struct Stretchable *window_skin;
  struct Stretchable *button_skin;
};

struct GUI {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  struct GUI_Skin *skin;
  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

struct GUI_Skin *load_gui_skin(struct Core *core, char *filename) {
  struct GUI_Skin *result = calloc(1, sizeof(struct GUI_Skin));
  struct JSON_Parser *parser = create_JSON_parser();
  struct Token *skin_json = parse_JSON_file(parser, filename);
  char *err = get_JSON_parser_err(parser);
  if (err) {
    WARNF("JSON err: %s", err);
    return NULL;
  }
  destroy_JSON_parser(parser);

  // Texture
  char *texture_filename = read_token_string(skin_json, "texture");
  result->texture = load_png(get_renderer(core), texture_filename);
  // Window
  struct Token *window_skin_json = read_token_token(skin_json, "window");
  result->window_skin = create_stretchable_from_token(window_skin_json);
  // Button
  struct Token *button_skin_json = read_token_token(skin_json, "button");
  result->button_skin = create_stretchable_from_token(button_skin_json);

  // Done
  destroy_token(skin_json);
  return result;
}

struct GUI *create_gui(struct Core *core, char *skin_filename) {
  struct GUI *result = calloc(1, sizeof(struct GUI));
  result->window = get_window(core);
  result->renderer = get_renderer(core);
  result->number_of_containers = 0;
  result->containers =
      calloc(MAX_GUI_CONTAINERS, sizeof(struct GUI_Container *));
  result->skin = load_gui_skin(core, skin_filename);
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

  for (unsigned int i = 0; i < gui->number_of_containers; i++)
    render_container(renderer, gui->containers[i], root_width, root_height,
                     gui->skin->texture, gui->skin->window_skin);
}
