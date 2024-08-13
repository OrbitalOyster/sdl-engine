#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Font {
  char *filename;
  TTF_Font *ttf;
  int size;
  TTF_Font *outline;
  int outline_size;
  // Must be set dynamically with SDL_QueryTexture
  unsigned int line_height;
};

struct Font *create_font(char *filename, int size, int outline_size);
void destroy_font(struct Font *font);

#endif
