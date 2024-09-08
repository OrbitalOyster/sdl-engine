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
    //    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    //    SDL_Rect tmp = (SDL_Rect){.x = left, .y = top, .w = w, .h = h};
    //    SDL_RenderDrawRect(renderer, &tmp);
    //    SDL_RenderDrawLine(renderer, left, top, right - 1, bottom - 1);
    //    SDL_RenderDrawLine(renderer, right, top, left, bottom - 1);

    // Coords
    SDL_Rect center_src = (SDL_Rect){.x = 32, .y = 32, .w = 16, .h = 16};
    SDL_Rect top_left_src = (SDL_Rect){.x = 16, .y = 16, .w = 16, .h = 16};
    SDL_Rect top_src = (SDL_Rect){.x = 32, .y = 16, .w = 16, .h = 16};
    SDL_Rect top_right_src = (SDL_Rect){.x = 48, .y = 16, .w = 16, .h = 16};
    SDL_Rect right_src = (SDL_Rect){.x = 48, .y = 32, .w = 16, .h = 16};
    SDL_Rect bottom_right_src = (SDL_Rect){.x = 48, .y = 48, .w = 16, .h = 16};
    SDL_Rect bottom_left_src = (SDL_Rect){.x = 16, .y = 48, .w = 16, .h = 16};
    SDL_Rect bottom_src = (SDL_Rect){.x = 32, .y = 48, .w = 16, .h = 16};
    SDL_Rect left_src = (SDL_Rect){.x = 16, .y = 32, .w = 16, .h = 16};

    // Center
    int cx = 0, cy = 0;
    while (cy < h - top_src.h - bottom_src.h) {
      while (cx < w - left_src.w - right_src.w) {
        SDL_Rect dst_rect = {.x = left + left_src.x + cx,
                             .y = top + top_src.y + cy,
                             .w = center_src.w,
                             .h = center_src.h};
        SDL_RenderCopy(renderer, gui->skin, &center_src, &dst_rect);
        cx += center_src.w;
      }
      cy += center_src.h;
      cx = 0;
    }

    // Top
    int tl = 0;
    while (tl < w - top_left_src.w - top_right_src.w) {
      SDL_Rect dst_rect = {.x = left + top_left_src.x + tl,
                           .y = top,
                           .w = top_src.w,
                           .h = top_src.h};
      SDL_RenderCopy(renderer, gui->skin, &top_src, &dst_rect);
      tl += top_src.w;
    }

    // Right
    int rl = 0;
    while (rl < h - top_right_src.h - bottom_right_src.h) {
      SDL_Rect dst_rect = {.x = right - right_src.h,
                           .y = top + top_right_src.h + rl,
                           .w = right_src.w,
                           .h = right_src.h};
      SDL_RenderCopy(renderer, gui->skin, &right_src, &dst_rect);
      rl += right_src.h;
    }

    // Bottom
    int bl = 0;
    while (bl < w - bottom_right_src.w - bottom_left_src.w) {
      SDL_Rect dst_rect = {.x = left + bottom_left_src.x + bl,
                           .y = bottom - bottom_src.h,
                           .w = bottom_src.w,
                           .h = bottom_src.h};
      SDL_RenderCopy(renderer, gui->skin, &bottom_src, &dst_rect);
      bl += bottom_src.w;
    }

    // Left
    int ll = 0;
    while (ll < h - top_left_src.h - bottom_left_src.h) {
      SDL_Rect dst_rect = {.x = left,
                           .y = top + top_left_src.h + ll,
                           .w = left_src.w,
                           .h = left_src.h};
      SDL_RenderCopy(renderer, gui->skin, &left_src, &dst_rect);
      ll += left_src.h;
    }

    // Top-left corner
    SDL_Rect dst_rect_0 = {
        .x = left, .y = top, .w = top_left_src.w, .h = top_left_src.h};
    SDL_RenderCopy(renderer, gui->skin, &top_left_src, &dst_rect_0);

    // Top-right corner
    SDL_Rect dst_rect_1 = {.x = right - top_right_src.w,
                           .y = top,
                           .w = top_right_src.w,
                           .h = top_right_src.h};
    SDL_RenderCopy(renderer, gui->skin, &top_right_src, &dst_rect_1);

    // Bottom-right corner
    SDL_Rect dst_rect_2 = {.x = right - top_right_src.w,
                           .y = bottom - bottom_right_src.h,
                           .w = bottom_right_src.w,
                           .h = bottom_right_src.h};
    SDL_RenderCopy(renderer, gui->skin, &bottom_right_src, &dst_rect_2);

    // Bottom-left corner
    SDL_Rect dst_rect_3 = {.x = left,
                           .y = bottom - bottom_left_src.h,
                           .w = bottom_left_src.w,
                           .h = bottom_left_src.h};
    SDL_RenderCopy(renderer, gui->skin, &bottom_left_src, &dst_rect_3);
  }
}
