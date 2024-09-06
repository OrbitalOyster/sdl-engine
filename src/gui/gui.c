#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "utils/debug.h"

#define MAX_GUI_CONTAINERS 255

struct GUI *create_gui(struct Core *core) {
  struct GUI *result = calloc(1, sizeof(struct GUI));
  result->core = core;
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
  case GUT_AUTO:
    ERR(1, "Attempt to measure auto GUI unit");
  case GUT_ABSOLUTE:
    return unit.px;
  case GUT_RELATIVE:
    return (int)round(unit.f * reference);
  default:
    ERR(1, "Invalid GUI unit type");
    return 0;
  }
}

/*
static int distance_to_px(struct GUI_Distance d, int ref1, int ref2) {
  int ref_px = 0;

  switch (d.ref_type) {
  case GUT_AUTO:
    ERRF(1, "Attempt to measure auto GUI unit %i %i", ref1, ref2);
  case GUT_ABSOLUTE:
    ref_px = d.ref_px;
    break;
  case GUT_RELATIVE:
    ref_px = (int)round(d.ref_pct * ref1);
    break;
  default: {
    ERR(1, "Invalid GUI unit type");
    return 0;
  }
  }

  switch (d.type) {
  case GUT_AUTO:
    ERR(1, "Attempt to measure auto GUI unit");
  case GUT_ABSOLUTE:
    return d.px - ref_px;
  case GUT_RELATIVE:
    return (int)round(d.f * ref2) - ref_px;
  default:
    ERR(1, "Invalid GUI unit type");
    return 0;
  }
}
*/

void render_gui(struct GUI *gui) {
  SDL_Window *window = get_window(gui->core);
  int root_width, root_height;
  SDL_GetWindowSize(window, &root_width, &root_height);

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
    int top_p = 0, right_p = 0, bottom_p = 0, left_p = 0;

    // Width
    if (c->width.type) {
      w = unit_to_px(c->width, root_width);

      if (c->right.type) {
        right_p = unit_to_px(c->right_p, w);
        right = root_width - unit_to_px(c->right, root_width) - right_p;
      }
      if (c->left.type) {
        left_p = unit_to_px(c->left_p, w);
        left = unit_to_px(c->left, root_width) - left_p;
      }

      if (!c->right.type)
        right = left + w;
      if (!c->left.type)
        left = right - w;

    } else {
      if (c->right_p.type != GUT_ABSOLUTE || c->left_p.type != GUT_ABSOLUTE)
        ERR(1, "Foo");
      right_p = unit_to_px(c->right_p, w);
      right = root_width - unit_to_px(c->right, root_width) - right_p;
      left_p = unit_to_px(c->left_p, w);
      left = unit_to_px(c->left, root_width) - left_p;
      w = right - left;
    }

    // Height
    if (c->height.type) {
      h = unit_to_px(c->height, root_height);

      if (c->top.type) {
        top_p = unit_to_px(c->top_p, h);
        top = unit_to_px(c->top, root_height) - top_p;
      }
      if (c->bottom.type) {
        bottom_p = unit_to_px(c->bottom_p, h);
        bottom = root_height - unit_to_px(c->bottom, root_height) - bottom_p;
      }

      if (!c->top.type)
        top = bottom - h;
      if (!c->bottom.type)
        bottom = top + h;
    } else {
      if (c->top_p.type != GUT_ABSOLUTE || c->bottom_p.type != GUT_ABSOLUTE)
        ERR(1, "Foo");
      top_p = unit_to_px(c->top_p, h);
      top = unit_to_px(c->top, root_height) - top_p;
      bottom_p = unit_to_px(c->bottom_p, h);
      bottom = root_height - unit_to_px(c->bottom, root_height) - bottom_p;
      h = bottom - top;
    }

    SDL_Renderer *renderer = get_renderer(gui->core);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect *tmp = calloc(1, sizeof(SDL_Rect));
    *tmp = (SDL_Rect){.x = left, .y = top, .w = w, .h = h};
    SDL_RenderDrawRect(renderer, tmp);

    SDL_RenderDrawLine(renderer, left, top, right - 1, bottom - 1);
    SDL_RenderDrawLine(renderer, right, top, left, bottom - 1);

    /*
    // Top anchor
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, left, top + top_anchor, left + w,
                       top + top_anchor);
    // Left anchor
    SDL_SetRenderDrawColor(renderer, 0xff, 0xcc, 0x22, 0xff);
    SDL_RenderDrawLine(renderer, left + left_anchor, top, left + left_anchor,
                       top + h);
    // Bottom anchor
    SDL_SetRenderDrawColor(renderer, 0x44, 0xff, 0x11, 0xff);
    SDL_RenderDrawLine(renderer, left, top + h - bottom_anchor, left + w,
                       top + h - bottom_anchor);
    // Right anchor
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);
    SDL_RenderDrawLine(renderer, left + w - right_anchor, top,
                       left + w - right_anchor, top + h);
                       */

    free(tmp);
  }
}
