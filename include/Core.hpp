#ifndef CORE_HPP_
#define CORE_HPP_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Core {
private:
  SDL_Window *window;

public:
  Core();
  void iterate();
  SDL_AppResult on_event(SDL_Event *event);
  SDL_Texture *load_png(const char *filename);
  SDL_Texture *hello;
  SDL_Renderer *renderer;
  ~Core();
};

#endif
