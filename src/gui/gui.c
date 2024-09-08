#include "gui/gui.h"

#include <math.h>
#include <stdlib.h>

#include "png.h"
#include "utils/debug.h"

#define MAX_GUI_CONTAINERS 255

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
  SDL_Window *window = gui->window;
  SDL_Renderer *renderer = gui->renderer;

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

    // Render
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect tmp = (SDL_Rect){.x = left, .y = top, .w = w, .h = h};
    SDL_RenderDrawRect(renderer, &tmp);
    SDL_RenderDrawLine(renderer, left, top, right - 1, bottom - 1);
    SDL_RenderDrawLine(renderer, right, top, left, bottom - 1);

    // Coords
    int x_0 = 16, y_0 = 16;
    int w_0 = 16, h_0 = 16;

    int x_top = 32, y_top = 16;
    int w_top = 16, h_top = 16;

    int x_1 = 48, y_1 = 16;
    int w_1 = 16, h_1 = 16;

    int x_2 = 48, y_2 = 48;
    int w_2 = 16, h_2 = 16;

    int x_3 = 16, y_3 = 48;
    int w_3 = 16, h_3 = 16;

    // Top
    int t = 0;
    SDL_Rect src_rect_01 = {.x = x_top, .y = y_top, .w = w_top, .h = h_top};
    while (t < w - w_0 - w_1) {
      SDL_Rect dst_rect = {.x = left + x_0 + t, .y = top, .w = w_top, .h = h_top};
      SDL_RenderCopy(renderer, gui->skin, &src_rect_01, &dst_rect);  
      t += w_top;
    }

    // Top-left corner
    SDL_Rect src_rect_0 = {.x = x_0, .y = y_0, .w = w_0, .h = h_0};
    SDL_Rect dst_rect_0 = {.x = left, .y = top, .w = w_0, .h = h_0};
    SDL_RenderCopy(renderer, gui->skin, &src_rect_0, &dst_rect_0);

    // Top-right corner
    SDL_Rect src_rect_1 = {.x = x_1, .y = y_1, .w = w_1, .h = h_1};
    SDL_Rect dst_rect_1 = {.x = right - w_1, .y = top, .w = w_1, .h = h_1};
    SDL_RenderCopy(renderer, gui->skin, &src_rect_1, &dst_rect_1);

    // Bottom-right corner
    SDL_Rect src_rect_2 = {.x = x_2, .y = y_2, .w = w_2, .h = h_2};
    SDL_Rect dst_rect_2 = {.x = right - w_1, .y = bottom - h_2, .w = w_2, .h = h_2};
    SDL_RenderCopy(renderer, gui->skin, &src_rect_2, &dst_rect_2);

    // Bottom-right corner
    SDL_Rect src_rect_3 = {.x = x_3, .y = y_3, .w = w_3, .h = h_3};
    SDL_Rect dst_rect_3 = {.x = left, .y = bottom - h_3, .w = w_3, .h = h_3};
    SDL_RenderCopy(renderer, gui->skin, &src_rect_3, &dst_rect_3);
  }
}
