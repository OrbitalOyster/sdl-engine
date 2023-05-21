#ifndef CAPTION_H
#define CAPTION_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"

typedef struct {
  // Position
  uint32_t x;
  uint32_t y;
  // Size
  uint32_t width;
  uint32_t height;
  // Text
  char *text;
  // Texture
  SDL_Texture *texture;
} Caption;

Caption *createCaption(SDL_Renderer *renderer, Font *font, uint32_t x,
                       uint32_t y, char *text, SDL_Color *color,
                       SDL_Color *outlineColor);

void destroyCaption(Caption *caption);

#endif
