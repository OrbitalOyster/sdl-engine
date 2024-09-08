#include "png.h"

#include "utils/debug.h"

SDL_Texture *load_png(SDL_Renderer *renderer, char *filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
  if (!texture) {
    WARNF("Failed to load asset: %s", SDL_GetError());
    return NULL;
  }
  return texture;
}
