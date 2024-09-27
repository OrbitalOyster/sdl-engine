#ifndef CONTAINER_H
#define CONTAINER_H

#include <SDL2/SDL.h>

#include "gui/unit.h"
#include "stretchable.h"

struct GUI_Window {
  struct GUI_Container *container;
};

struct GUI_Button {
  struct GUI_Container *container;
};

struct GUI_Container {
  struct GUI_Unit top;
  struct GUI_Unit top_p;
  struct GUI_Unit right;
  struct GUI_Unit right_p;
  struct GUI_Unit bottom;
  struct GUI_Unit bottom_p;
  struct GUI_Unit left;
  struct GUI_Unit left_p;
  struct GUI_Unit width;
  struct GUI_Unit height;

  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

void render_container(SDL_Renderer *renderer, struct GUI_Container *c,
                      int root_width, int root_height, SDL_Texture *skin,
                      struct Stretchable *stretchable);

#endif /* CONTAINER_H */
