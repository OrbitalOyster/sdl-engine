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

    // Width
    int w = 0;
    if (c->width.type == GUT_ABSOLUTE)
      w = c->width.px;
    if (c->width.type == GUT_RELATIVE)
      w = (int)round(root_width * c->width.pct);

    // Height
    int h = 0;
    if (c->height.type == GUT_ABSOLUTE)
      h = c->height.px;
    if (c->height.type == GUT_RELATIVE)
      h = (int)round(root_height * c->height.pct);

    // Top
    int top = 0;
    if (c->top_distance.type == GUT_ABSOLUTE)
      top = c->top_distance.px;
    else
      top = (int)round(root_height * c->top_distance.pct);
    int top_anchor = 0;
    if (c->top_anchor.type == GUT_ABSOLUTE)
      top_anchor = c->top_anchor.px;
    else 
      top_anchor = (int)round(c->top_anchor.pct * h);
    top -= top_anchor;

    // Left
    int left = 0;
    if (c->left_distance.type == GUT_ABSOLUTE)
      left = c->left_distance.px;
    else
      left = (int)round(root_width * c->left_distance.pct);
    int left_anchor = 0;
    if (c->left_anchor.type == GUT_ABSOLUTE)
      left_anchor = c->left_anchor.px;
    else
      left_anchor = (int)round(c->left_anchor.pct * w);
    left -= left_anchor;

    // Bottom
    int bottom = 0;
    if (c->bottom_distance.type == GUT_ABSOLUTE) {
      if (!h)
        bottom = root_height - c->bottom_distance.px;
      else
        bottom = top + h;
    } else
      bottom = (int)round(root_height * c->bottom_distance.pct);
    int bottom_anchor = 0;
    if (c->bottom_anchor.type == GUT_ABSOLUTE)
      bottom_anchor = c->bottom_anchor.px;
    else
      bottom_anchor = (int)round(c->bottom_anchor.pct * h);
    bottom += bottom_anchor;

    // Right
    int right = 0;
    if (c->right_distance.type == GUT_ABSOLUTE) {
      if (!w)
        right = root_width - c->right_distance.px;
      else
        right = left + w;
    } else
      right = (int)round(root_width * c->right_distance.pct);
    int right_anchor = 0;
    if (c->right_anchor.type == GUT_ABSOLUTE)
      right_anchor = c->right_anchor.px;
    else
      right_anchor = (int)round(c->right_anchor.pct * w);
    right += right_anchor;

    // Width
    if (!w)
      w = right - left;
    // Height
    if (!h)
      h = bottom - top;

    SDL_Renderer *renderer = get_renderer(gui->core);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect *tmp = calloc(1, sizeof(SDL_Rect));
    *tmp = (SDL_Rect){.x = left, .y = top, .w = w, .h = h};
    SDL_RenderDrawRect(renderer, tmp);

    SDL_RenderDrawLine(renderer, left, top, left + w - 1, top + h - 1);
    SDL_RenderDrawLine(renderer, left + w - 1, top, left, top + h - 1);

    // Top anchor
    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, left, top + top_anchor, right - 1,
                       top + top_anchor);
    // Left anchor
    SDL_RenderDrawLine(renderer, left + left_anchor, top, left + left_anchor,
                       bottom - 1);
    // Bottom anchor
    SDL_RenderDrawLine(renderer, left, bottom - bottom_anchor - 1, right,
                       bottom - bottom_anchor - 1);
    // Right anchor
    SDL_RenderDrawLine(renderer, right - right_anchor - 1, top,
                       right - right_anchor - 1, bottom);

    free(tmp);
  }
}
