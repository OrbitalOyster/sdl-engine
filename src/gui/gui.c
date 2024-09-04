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
    return (int)round(unit.pct * reference);
  default:
    ERR(1, "Invalid GUI unit type");
    return 0;
  }
}

void render_gui(struct GUI *gui) {
  SDL_Window *window = get_window(gui->core);
  int root_width, root_height;
  SDL_GetWindowSize(window, &root_width, &root_height);

  for (unsigned int i = 0; i < gui->number_of_containers; i++) {
    struct GUI_Container *c = gui->containers[i];

    // Check types
    if (c->width.type + c->right_distance.type + c->left_distance.type <
            GUT_ABSOLUTE * 2 ||
        (c->width.type && c->right_distance.type && c->left_distance.type))
      ERRF(1, "Invalid GUI types: %u %u %u", c->width.type,
           c->right_distance.type, c->left_distance.type);
    if (c->height.type + c->top_distance.type + c->bottom_distance.type <
            GUT_ABSOLUTE * 2 ||
        (c->height.type && c->top_distance.type && c->bottom_distance.type))
      ERRF(1, "Invalid GUI types: %u %u %u", c->height.type,
           c->top_distance.type, c->bottom_distance.type);

    // Top
    int top = 0;
    if (c->top_distance.type)
      top = unit_to_px(c->top_distance, root_height);

    // Right
    int right = 0;
    if (c->right_distance.type)
      right = root_width - unit_to_px(c->right_distance, root_width);

    // Bottom
    int bottom = 0;
    if (c->bottom_distance.type)
      bottom = root_height - unit_to_px(c->bottom_distance, root_height);

    // Left
    int left = 0;
    if (c->left_distance.type)
      left = unit_to_px(c->left_distance, root_width);

    // Width
    int w = c->width.type ? unit_to_px(c->width, root_width) : right - left;
    // Height
    int h = c->height.type ? unit_to_px(c->height, root_height) : bottom - top;

    // Top
    if (c->top_distance.type == GUT_AUTO)
      top = bottom - h;
    // Left
    if (c->left_distance.type == GUT_AUTO)
      left = right - w;
    // Bottom
    if (c->bottom_distance.type == GUT_AUTO)
      bottom = top + h;
    // Right
    if (c->right_distance.type == GUT_AUTO)
      right = left + w;

    // Anchors
    int top_anchor = 0;
    if (c->top_anchor.type)
      top_anchor = unit_to_px(c->top_anchor, h);
    else
      ERR(1, "Invalid anchor type");

    if (c->top_distance.type) {
      top -= top_anchor;
      //      bottom -= top_anchor;
    }

    int right_anchor = 0;
    if (c->right_anchor.type)
      right_anchor = unit_to_px(c->right_anchor, w);
    else
      ERR(1, "Invalid anchor type");

    if (c->right_distance.type) {
      //      right += right_anchor;
      left += right_anchor;
    }

    int bottom_anchor = 0;
    if (c->bottom_anchor.type)
      bottom_anchor = unit_to_px(c->bottom_anchor, h);
    else
      ERR(1, "Invalid anchor type");

    if (c->bottom_distance.type) {
      top += bottom_anchor;
      //      bottom += bottom_anchor;
    }

    int left_anchor = 0;
    if (c->left_anchor.type)
      left_anchor = unit_to_px(c->left_anchor, w);
    else
      ERR(1, "Invalid anchor type");

    if (c->left_distance.type) {
      //      right -= left_anchor;
      left -= left_anchor;
    }

    //    INFO2F("w:%i h:%i top: %i right: %i bottom: %i left %i", w, h, top,
    //    right,
    //           bottom, left);

    SDL_Renderer *renderer = get_renderer(gui->core);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect *tmp = calloc(1, sizeof(SDL_Rect));
    *tmp = (SDL_Rect){.x = left, .y = top, .w = w, .h = h};
    SDL_RenderDrawRect(renderer, tmp);

    SDL_RenderDrawLine(renderer, left, top, left + w - 1, top + h - 1);
    SDL_RenderDrawLine(renderer, left + w - 1, top, left, top + h - 1);

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

    free(tmp);
  }
}
