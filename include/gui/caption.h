#ifndef CAPTION_H
#define CAPTION_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "font.h"

#define MAX_CAPTION_LINES 255
#define MAX_CAPTION_LINE_SIZE 255

struct Caption {
  int x;
  int y;
  char *text;
  unsigned int number_of_lines;
  char **lines;
  struct Font *font;
  SDL_Color *color;
  SDL_Color *outline_color;
  // Space between lines
  int leading;
  SDL_Texture **textures;
};

struct Caption *create_caption(SDL_Renderer *renderer, int x, int y, char *text,
                               struct Font *font, SDL_Color *color,
                               SDL_Color *outline_color);
void update_caption_texture(SDL_Renderer *renderer, struct Caption *caption);
void destroy_caption(struct Caption *caption);

#endif
