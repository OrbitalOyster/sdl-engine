#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include "png.h"
#include "utils/debug.h"

#define MAX_GUI_CONTAINERS 255

struct GUI *create_gui(struct Core *core, char *skin) {
  struct GUI *result = calloc(1, sizeof(struct GUI));
  result->core = core;
  result->skin = load_png(get_renderer(core), skin);
  result->number_of_containers = 0;
  result->containers =
      calloc(MAX_GUI_CONTAINERS, sizeof(struct GUI_Container *));
  return result;
}

void add_gui_container(struct GUI *gui, struct GUI_Container *container) {
  gui->containers[gui->number_of_containers++] = container;
}

static int unit_to_px(struct GUI_Unit unit, int reference) {
  switch (unit.type) {
  case GUT_NONE:
    return 0;
  case GUT_ABSOLUTE:
    return unit.px;
  case GUT_RELATIVE:
    return (int)round(unit.f * reference);
  default:
    ERR(1, "Invalid GUI unit type");
    return 0;
  }
}

void render_gui(struct GUI *gui) {
  SDL_Window *window = get_window(gui->core);
  SDL_Renderer *renderer = get_renderer(gui->core);

  int root_width, root_height;
  SDL_GetWindowSize(window, &root_width, &root_height);

  SDL_Rect dstrect = {.x = 100, .y = 300, .w = 512, .h = 512};
  SDL_RenderCopy(renderer, gui->skin, NULL, &dstrect);

  for (unsigned int i = 0; i < gui->number_of_containers; i++) {
    struct GUI_Container *c = gui->containers[i];

    // Check types
    if (c->width.type + c->right.type + c->left.type < GUT_ABSOLUTE * 2 ||
        (c->width.type && c->right.type && c->left.type))
      ERRF(1, "Invalid GUI horizontal types: %u %u %u", c->width.type,
           c->right.type, c->left.type);
    if (c->height.type + c->top.type + c->bottom.type < GUT_ABSOLUTE * 2 ||
        (c->height.type && c->top.type && c->bottom.type))
      ERRF(1, "Invalid GUI vertical types: %u %u %u", c->height.type,
           c->top.type, c->bottom.type);

    int w = 0, h = 0;
    int top = 0, right = 0, bottom = 0, left = 0;

    // Horizontal axis
    if (c->width.type) {
      w = unit_to_px(c->width, root_width);
      if (c->right.type)
        right = root_width - unit_to_px(c->right, root_width) +
                unit_to_px(c->right_p, w);
      if (c->left.type)
        left = unit_to_px(c->left, root_width) - unit_to_px(c->left_p, w);
      if (!c->right.type)
        right = left + w;
      if (!c->left.type)
        left = right - w;

    } else {
      if (c->right_p.type == GUT_RELATIVE || c->left_p.type == GUT_RELATIVE)
        ERR(1, "P-type relative");
      right = root_width - unit_to_px(c->right, root_width) +
              unit_to_px(c->right_p, w);
      left = unit_to_px(c->left, root_width) - unit_to_px(c->left_p, w);
      w = right - left;
    }

    // Vertical axis
    if (c->height.type) {
      h = unit_to_px(c->height, root_height);
      if (c->top.type)
        top = unit_to_px(c->top, root_height) - unit_to_px(c->top_p, h);
      if (c->bottom.type)
        bottom = root_height - unit_to_px(c->bottom, root_height) +
                 unit_to_px(c->bottom_p, h);
      if (!c->top.type)
        top = bottom - h;
      if (!c->bottom.type)
        bottom = top + h;
    } else {
      if (c->top_p.type == GUT_RELATIVE || c->bottom_p.type == GUT_RELATIVE)
        ERR(1, "P-type relative");
      top = unit_to_px(c->top, root_height) - unit_to_px(c->top_p, h);
      bottom = root_height - unit_to_px(c->bottom, root_height) +
               unit_to_px(c->bottom_p, h);
      h = bottom - top;
    }

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect *tmp = calloc(1, sizeof(SDL_Rect));
    *tmp = (SDL_Rect){.x = left, .y = top, .w = w, .h = h};
    SDL_RenderDrawRect(renderer, tmp);

    SDL_RenderDrawLine(renderer, left, top, right - 1, bottom - 1);
    SDL_RenderDrawLine(renderer, right, top, left, bottom - 1);

    free(tmp);
  }
}
