#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include "png.h"
#include "stretchable.h"
#include "JSON/parser.h"
#include "JSON/token.h"

#include "utils/debug.h"

#define MAX_GUI_CAPTIONS 255
#define MAX_GUI_WINDOWS 255
#define MAX_GUI_BUTTONS 255

struct GUI_Skin {
  SDL_Texture *texture;
  struct Stretchable *window_skin;
  struct Stretchable *button_skin;
};

struct GUI {
  SDL_Window *core_window;
  SDL_Renderer *renderer;
  /*SDL_Texture *texture;*/
  struct GUI_Skin *skin;

  unsigned int number_of_captions;
  struct GUI_Caption **captions;

  unsigned int number_of_windows;
  struct GUI_Window **windows;

  unsigned int number_of_buttons;
  struct GUI_Button **buttons;
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
  result->core_window = get_window(core);
  result->renderer = get_renderer(core);
  result->number_of_captions = 0;
  result->number_of_windows = 0;
  result->number_of_buttons = 0;
  result->windows = calloc(MAX_GUI_WINDOWS, sizeof(struct GUI_Window *));
  result->buttons = calloc(MAX_GUI_BUTTONS, sizeof(struct GUI_Button *));
  result->skin = load_gui_skin(core, skin_filename);
  return result;
}

void add_gui_caption(struct GUI *gui, struct GUI_Container *parent,
                    struct GUI_Caption *caption) {
  if (parent)
    parent->containers[parent->number_of_containers++] = caption->container;
  gui->captions[gui->number_of_captions++] = caption;
}

void add_gui_window(struct GUI *gui, struct GUI_Container *parent,
                    struct GUI_Window *window) {
  window->container->texture = gui->skin->texture;
  window->container->stretchable = gui->skin->window_skin;
  if (parent)
    parent->containers[parent->number_of_containers++] = window->container;
  gui->windows[gui->number_of_windows++] = window;
}

void add_gui_button(struct GUI *gui, struct GUI_Container *parent,
                    struct GUI_Button *button) {
  button->container->texture = gui->skin->texture;
  button->container->stretchable = gui->skin->button_skin;
  if (parent)
    parent->containers[parent->number_of_containers++] = button->container;
  gui->buttons[gui->number_of_buttons++] = button;
}

void render_gui(struct GUI *gui) {
  SDL_Renderer *renderer = gui->renderer;

  int root_width, root_height;
  SDL_GetWindowSize(gui->core_window, &root_width, &root_height);

  // Windows
  for (unsigned int i = 0; i < gui->number_of_windows; i++)
    render_container(renderer, gui->windows[i]->container, root_width,
                     root_height);
  // Buttons
  /*for (unsigned int i = 0; i < gui->number_of_buttons; i++)*/
  /*  render_container(renderer, gui->buttons[i]->container, root_width,*/
  /*                   root_height);*/
}
