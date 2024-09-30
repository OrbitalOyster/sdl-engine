#include "gui/container.h"

#include "gui/unit.h"
#include "utils/debug.h"

struct GUI_Component *create_component(enum GUI_Component_Type type) {
  struct GUI_Component *result = calloc(1, sizeof(struct GUI_Component));
  result->type = type;
  return result;
}

void render_container(SDL_Renderer *renderer, struct GUI_Container *c,
                      int root_width, int root_height) {

  struct Stretchable *stretchable = c->stretchable;

  // Check types
  if (c->width.type + c->right.type + c->left.type < GUT_ABSOLUTE * 2 ||
      (c->width.type && c->right.type && c->left.type))
    ERRF(1, "Invalid GUI horizontal types: %u %u %u", c->width.type,
         c->right.type, c->left.type);
  if (c->height.type + c->top.type + c->bottom.type < GUT_ABSOLUTE * 2 ||
      (c->height.type && c->top.type && c->bottom.type))
    ERRF(1, "Invalid GUI vertical types: %u %u %u", c->height.type, c->top.type,
         c->bottom.type);

  int width = 0, height = 0;
  int top = 0, right = 0, bottom = 0, left = 0;

  // Horizontal axis
  if (c->width.type) {
    width = unit_to_px(c->width, root_width);
    if (c->right.type)
      right = root_width - unit_to_px(c->right, root_width) +
              unit_to_px(c->right_p, width);
    if (c->left.type)
      left = unit_to_px(c->left, root_width) - unit_to_px(c->left_p, width);
    if (!c->right.type)
      right = left + width;
    if (!c->left.type)
      left = right - width;

  } else {
    if (c->right_p.type == GUT_RELATIVE || c->left_p.type == GUT_RELATIVE)
      ERR(1, "P-type relative");
    right = root_width - unit_to_px(c->right, root_width) +
            unit_to_px(c->right_p, width);
    left = unit_to_px(c->left, root_width) - unit_to_px(c->left_p, width);
    width = right - left;
  }

  // Vertical axis
  if (c->height.type) {
    height = unit_to_px(c->height, root_height);
    if (c->top.type)
      top = unit_to_px(c->top, root_height) - unit_to_px(c->top_p, height);
    if (c->bottom.type)
      bottom = root_height - unit_to_px(c->bottom, root_height) +
               unit_to_px(c->bottom_p, height);
    if (!c->top.type)
      top = bottom - height;
    if (!c->bottom.type)
      bottom = top + height;
  } else {
    if (c->top_p.type == GUT_RELATIVE || c->bottom_p.type == GUT_RELATIVE)
      ERR(1, "P-type relative");
    top = unit_to_px(c->top, root_height) - unit_to_px(c->top_p, height);
    bottom = root_height - unit_to_px(c->bottom, root_height) +
             unit_to_px(c->bottom_p, height);
    height = bottom - top;
  }

  // Center
  int cx = 0, cy = 0;
  while (cy < height - stretchable->top.h - stretchable->bottom.h) {
    while (cx < width - stretchable->left.w - stretchable->right.w) {
      SDL_Rect dst_rect = {.x = left + stretchable->top_left.w + cx,
                           .y = top + stretchable->top_left.h + cy,
                           .w = stretchable->center.w,
                           .h = stretchable->center.h};
      SDL_RenderCopy(renderer, c->texture, &stretchable->center, &dst_rect);
      cx += stretchable->center.w;
    }
    cy += stretchable->center.h;
    cx = 0;
  }

  // Top
  int tl = 0;
  while (tl < width - stretchable->top_left.w - stretchable->top_right.w) {
    SDL_Rect dst_rect = {.x = left + stretchable->top_left.w + tl,
                         .y = top,
                         .w = stretchable->top.w,
                         .h = stretchable->top.h};
    SDL_RenderCopy(renderer, c->texture, &stretchable->top, &dst_rect);
    tl += stretchable->top.w;
  }

  // Right
  int rl = 0;
  while (rl < height - stretchable->top_right.h - stretchable->bottom_right.h) {
    SDL_Rect dst_rect = {.x = right - stretchable->right.h,
                         .y = top + stretchable->top_right.h + rl,
                         .w = stretchable->right.w,
                         .h = stretchable->right.h};
    SDL_RenderCopy(renderer, c->texture, &stretchable->right, &dst_rect);
    rl += stretchable->right.h;
  }

  // Bottom
  int bl = 0;
  while (bl <
         width - stretchable->bottom_right.w - stretchable->bottom_left.w) {
    SDL_Rect dst_rect = {.x = left + stretchable->bottom_left.h + bl,
                         .y = bottom - stretchable->bottom.h,
                         .w = stretchable->bottom.w,
                         .h = stretchable->bottom.h};
    SDL_RenderCopy(renderer, c->texture, &stretchable->bottom, &dst_rect);
    bl += stretchable->bottom.w;
  }

  // Left
  int ll = 0;
  while (ll < height - stretchable->top_left.h - stretchable->bottom_left.h) {
    SDL_Rect dst_rect = {.x = left,
                         .y = top + stretchable->top_left.h + ll,
                         .w = stretchable->left.w,
                         .h = stretchable->left.h};
    SDL_RenderCopy(renderer, c->texture, &stretchable->left, &dst_rect);
    ll += stretchable->left.h;
  }

  // Top-left corner
  SDL_Rect dst_rect_0 = {.x = left,
                         .y = top,
                         .w = stretchable->top_left.w,
                         .h = stretchable->top_left.h};
  SDL_RenderCopy(renderer, c->texture, &stretchable->top_left, &dst_rect_0);

  // Top-right corner
  SDL_Rect dst_rect_1 = {.x = right - stretchable->top_right.w,
                         .y = top,
                         .w = stretchable->top_right.w,
                         .h = stretchable->top_right.h};
  SDL_RenderCopy(renderer, c->texture, &stretchable->top_right, &dst_rect_1);

  // Bottom-right corner
  SDL_Rect dst_rect_2 = {.x = right - stretchable->top_right.w,
                         .y = bottom - stretchable->bottom_right.h,
                         .w = stretchable->bottom_right.w,
                         .h = stretchable->bottom_right.h};
  SDL_RenderCopy(renderer, c->texture, &stretchable->bottom_right, &dst_rect_2);

  // Bottom-left corner
  SDL_Rect dst_rect_3 = {.x = left,
                         .y = bottom - stretchable->bottom_left.h,
                         .w = stretchable->bottom_left.w,
                         .h = stretchable->bottom_left.h};
  SDL_RenderCopy(renderer, c->texture, &stretchable->bottom_left, &dst_rect_3);

  for (unsigned int i = 0; i < c->number_of_containers; i++)
    render_container(renderer, c->containers[i], width, height);
}
