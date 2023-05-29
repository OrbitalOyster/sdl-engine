#ifndef CAPTION_H
#define CAPTION_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"

#define MAX_CAPTION_LINES 255
#define MAX_CAPTION_LINE_SIZE 255

typedef struct {
  int32_t x;
  int32_t y;
  char *text;
  uint8_t numberOfLines;
  char **lines;
  Font *font;
  SDL_Color *color;
  SDL_Color *outlineColor;
  // Space between lines
  int8_t leading;
  SDL_Texture **textures;
} Caption;

Caption *createCaption(SDL_Renderer *renderer, int32_t x, int32_t y,
                       char *text, Font *font, SDL_Color *color,
                       SDL_Color *outlineColor);
void updateCaptionTexture(SDL_Renderer *renderer, Caption *caption);
void destroyCaption(Caption *caption);

#endif
