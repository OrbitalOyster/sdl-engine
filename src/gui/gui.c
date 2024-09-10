#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include "png.h"
#include "utils/debug.h"

#define MAX_GUI_CONTAINERS 255

struct GUI {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *skin;
  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

struct GUI *create_gui(struct Core *core, char *skin) {
  struct GUI *result = calloc(1, sizeof(struct GUI));
  result->window = get_window(core);
  result->renderer = get_renderer(core);
  result->skin = load_png(get_renderer(core), skin);
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

  for (unsigned int i = 0; i < gui->number_of_containers; i++)
    render_container(renderer, gui->containers[i], root_width, root_height,
                     gui->skin);
}
