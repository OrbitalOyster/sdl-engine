#ifndef CONTAINER_H
#define CONTAINER_H

#include <SDL2/SDL.h>

#include "gui/unit.h"
#include "stretchable.h"

struct GUI_Caption {
  struct GUI_Container *container;
  char *text;
};

struct GUI_Window {
  struct GUI_Container *container;
};

struct GUI_Button {
  struct GUI_Container *container;
};

/**
 * _p means "padding", i.e. offset distance to according edge
 */
struct GUI_Container {
  SDL_Texture *texture;
  struct Stretchable *stretchable;
  struct GUI_Unit top; // Distance to parent container's top edge
  struct GUI_Unit top_p;
  struct GUI_Unit right; // Distance to parent container's right edge
  struct GUI_Unit right_p;
  struct GUI_Unit bottom; // Distance to parent container's bottom edge
  struct GUI_Unit bottom_p;
  struct GUI_Unit left; // Distance to parent container's left edge
  struct GUI_Unit left_p;
  struct GUI_Unit width;
  struct GUI_Unit height;

  struct GUI_Container *parent;
  unsigned int number_of_containers;
  struct GUI_Container **containers;
};

enum GUI_Component_Type {GCT_Window, GCT_Button};

struct GUI_Component {
  enum GUI_Component_Type type;
  struct GUI_Container *container;
};

struct GUI_Component *create_component(enum GUI_Component_Type type);

void render_container(SDL_Renderer *renderer, struct GUI_Container *c,
                      int root_width, int root_height);

#endif /* CONTAINER_H */
