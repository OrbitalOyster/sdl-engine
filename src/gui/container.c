#include "gui/container.h"

#include "gui/unit.h"
#include "utils/debug.h"

void render_container(SDL_Renderer *renderer, struct GUI_Container *c,
                      int root_width, int root_height, SDL_Texture *skin,
                      struct Stretchable *foo) {
  // Check types
  if (c->width.type + c->right.type + c->left.type < GUT_ABSOLUTE * 2 ||
      (c->width.type && c->right.type && c->left.type))
    ERRF(1, "Invalid GUI horizontal types: %u %u %u", c->width.type,
         c->right.type, c->left.type);
  if (c->height.type + c->top.type + c->bottom.type < GUT_ABSOLUTE * 2 ||
      (c->height.type && c->top.type && c->bottom.type))
    ERRF(1, "Invalid GUI vertical types: %u %u %u", c->height.type, c->top.type,
         c->bottom.type);

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
  /*
  SDL_Rect center_src = (SDL_Rect){.x = 32, .y = 32, .w = 16, .h = 16};
  SDL_Rect top_src = (SDL_Rect){.x = 32, .y = 16, .w = 16, .h = 16};
  SDL_Rect right_src = (SDL_Rect){.x = 48, .y = 32, .w = 16, .h = 16};
  SDL_Rect bottom_src = (SDL_Rect){.x = 32, .y = 48, .w = 16, .h = 16};
  SDL_Rect left_src = (SDL_Rect){.x = 16, .y = 32, .w = 16, .h = 16};
  SDL_Rect top_left_src = (SDL_Rect){.x = 16, .y = 16, .w = 16, .h = 16};
  SDL_Rect top_right_src = (SDL_Rect){.x = 48, .y = 16, .w = 16, .h = 16};
  SDL_Rect bottom_right_src = (SDL_Rect){.x = 48, .y = 48, .w = 16, .h = 16};
  SDL_Rect bottom_left_src = (SDL_Rect){.x = 16, .y = 48, .w = 16, .h = 16};
  */

  // Center
  int cx = 0, cy = 0;
  while (cy < h - foo->top.h - foo->bottom.h) {
    while (cx < w - foo->left.w - foo->right.w) {
      SDL_Rect dst_rect = {.x = left + foo->left.x + cx,
                           .y = top + foo->top.y + cy,
                           .w = foo->center.w,
                           .h = foo->center.h};
      SDL_RenderCopy(renderer, skin, &foo->center, &dst_rect);
      cx += foo->center.w;
    }
    cy += foo->center.h;
    cx = 0;
  }

  // Top
  int tl = 0;
  while (tl < w - foo->top_left.w - foo->top_right.w) {
    SDL_Rect dst_rect = {.x = left + foo->top_left.x + tl,
                         .y = top,
                         .w = foo->top.w,
                         .h = foo->top.h};
    SDL_RenderCopy(renderer, skin, &foo->top, &dst_rect);
    tl += foo->top.w;
  }

  // Right
  int rl = 0;
  while (rl < h - foo->top_right.h - foo->bottom_right.h) {
    SDL_Rect dst_rect = {.x = right - foo->right.h,
                         .y = top + foo->top_right.h + rl,
                         .w = foo->right.w,
                         .h = foo->right.h};
    SDL_RenderCopy(renderer, skin, &foo->right, &dst_rect);
    rl += foo->right.h;
  }

  // Bottom
  int bl = 0;
  while (bl < w - foo->bottom_right.w - foo->bottom_left.w) {
    SDL_Rect dst_rect = {.x = left + foo->bottom_left.x + bl,
                         .y = bottom - foo->bottom.h,
                         .w = foo->bottom.w,
                         .h = foo->bottom.h};
    SDL_RenderCopy(renderer, skin, &foo->bottom, &dst_rect);
    bl += foo->bottom.w;
  }

  // Left
  int ll = 0;
  while (ll < h - foo->top_left.h - foo->bottom_left.h) {
    SDL_Rect dst_rect = {.x = left,
                         .y = top + foo->top_left.h + ll,
                         .w = foo->left.w,
                         .h = foo->left.h};
    SDL_RenderCopy(renderer, skin, &foo->left, &dst_rect);
    ll += foo->left.h;
  }

  // Top-left corner
  SDL_Rect dst_rect_0 = {
      .x = left, .y = top, .w = foo->top_left.w, .h = foo->top_left.h};
  SDL_RenderCopy(renderer, skin, &foo->top_left, &dst_rect_0);

  // Top-right corner
  SDL_Rect dst_rect_1 = {.x = right - foo->top_right.w,
                         .y = top,
                         .w = foo->top_right.w,
                         .h = foo->top_right.h};
  SDL_RenderCopy(renderer, skin, &foo->top_right, &dst_rect_1);

  // Bottom-right corner
  SDL_Rect dst_rect_2 = {.x = right - foo->top_right.w,
                         .y = bottom - foo->bottom_right.h,
                         .w = foo->bottom_right.w,
                         .h = foo->bottom_right.h};
  SDL_RenderCopy(renderer, skin, &foo->bottom_right, &dst_rect_2);

  // Bottom-left corner
  SDL_Rect dst_rect_3 = {.x = left,
                         .y = bottom - foo->bottom_left.h,
                         .w = foo->bottom_left.w,
                         .h = foo->bottom_left.h};
  SDL_RenderCopy(renderer, skin, &foo->bottom_left, &dst_rect_3);
}
