#include "render.h"

#include "debug.h"

void drawRect(SDL_Renderer *renderer, double x, double y, double w, double h) {
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_FRect *tmp = calloc(1, sizeof(SDL_FRect));
  *tmp =
      (SDL_FRect){.x = (float)x, .y = (float)y, .w = (float)w, .h = (float)h};
  SDL_RenderDrawRectF(renderer, tmp);
  free(tmp);
}

void renderProp(SDL_Renderer *renderer, Prop *prop) {
  drawRect(renderer, prop->x, prop->y, prop->w, prop->h);
  SDL_RenderDrawLineF(renderer, (float)prop->x, (float)prop->y,
                      (float)(prop->x + prop->w - 1), (float)(prop->y + prop->h - 1));
  SDL_RenderDrawLineF(renderer, (float)(prop->x + prop->w - 1), (float)prop->y,
                      (float)prop->x, (float)(prop->y + prop->h - 1));
}

void renderEntity(SDL_Renderer *renderer, Entity *entity) {
  drawRect(renderer, entity->x, entity->y, entity->w, entity->h);
}

