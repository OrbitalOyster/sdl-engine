#include "prop.h"

#include <stdlib.h>

Prop *createProp(double x, double y, double w, double h) {
  Prop *prop = calloc(1, sizeof(Prop));
  *prop = (Prop) {.x = x, .y = y, .w = w, .h = h};
  prop->rect = createOrthoRect(x, y, w, h);
  return prop;
}

/*
void renderProp(Prop *prop, SDL_Renderer *renderer, bool debug) {
  if (debug) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_Rect *tmp_rect = calloc(1, sizeof(SDL_Rect));
    tmp_rect->x = prop->x;
    tmp_rect->y = prop->y;
    tmp_rect->w = prop->w;
    tmp_rect->h = prop->h;
    SDL_RenderDrawRect(renderer, tmp_rect);

    if (prop->debug_flash) {
      if (prop->debug_flash == 1)
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
      if (prop->debug_flash == 2)
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
      SDL_RenderFillRect(renderer, tmp_rect);
      prop->debug_flash = 0;
    }

    free(tmp_rect);
  }
}
*/

void freeProp(Prop *prop) {
  freeOrthoRect(prop->rect);
  free(prop);
}

