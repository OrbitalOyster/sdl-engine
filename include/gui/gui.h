#ifndef GUI_H
#define GUI_H

#include "core.h"
#include "gui/container.h"
#include "gui/unit.h"

struct GUI {
  SDL_Window *window;
  SDL_Renderer * renderer;
  SDL_Texture *skin;
  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

struct GUI *create_gui(struct Core *core, char *skin);
void add_gui_container(struct GUI *gui, struct GUI_Container *container);
void render_gui(struct GUI *gui);

#endif /* GUI_H */
