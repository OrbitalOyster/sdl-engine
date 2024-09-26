#ifndef STRETCHABLE_H
#define STRETCHABLE_H

#include <SDL2/SDL.h>

#include "JSON/token.h"

struct Stretchable {
  SDL_Rect center;
  SDL_Rect top;
  SDL_Rect right;
  SDL_Rect bottom;
  SDL_Rect left;
  SDL_Rect top_left;
  SDL_Rect top_right;
  SDL_Rect bottom_right;
  SDL_Rect bottom_left;
};

struct Stretchable *create_stretchable_from_token(struct Token *token);

#endif /* STRETCHABLE_H */
