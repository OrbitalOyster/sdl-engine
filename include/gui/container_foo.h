#ifndef CONTAINER_FOO_H
#define CONTAINER_FOO_H

#include <SDL2/SDL.h>

struct Container_Foo {
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

#endif
