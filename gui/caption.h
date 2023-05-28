#ifndef CAPTION_H
#define CAPTION_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"

#define MAX_CAPTION_LINES 255
#define MAX_CAPTION_LINE_SIZE 255

typedef struct {
  uint32_t x;
  uint32_t y;
  char *text;
  uint8_t lines;
  Font *font;
  SDL_Color *color;
  SDL_Color *outlineColor;
  // Size is determined dynamically
  uint32_t w;
  uint32_t h;
  SDL_Texture *texture;
  SDL_Texture **textures;
} Caption;

Caption *createCaption(SDL_Renderer *renderer, uint32_t x, uint32_t y,
                       char *text, Font *font, SDL_Color *color,
                       SDL_Color *outlineColor);
void updateCaptionTexture(SDL_Renderer *renderer, Caption *caption);
void destroyCaption(Caption *caption);

#endif
