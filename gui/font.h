#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
  char *filename;
  TTF_Font *ttf;
  uint8_t size;
  TTF_Font *outline;
  uint8_t outlineSize;
  // Must be set dynamically with SDL_QueryTexture
  uint8_t lineHeight;
} Font;

Font *createFont(char *filename, uint8_t size, uint8_t outlineSize);
void destroyFont(Font *font);

#endif
