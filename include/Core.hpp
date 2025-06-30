#ifndef CORE_HPP_
#define CORE_HPP_

#include <SDL3/SDL.h>

class Core {
private:
  SDL_Window *window;
  SDL_Renderer *renderer;

public:
  Core();
  void iterate();
  SDL_AppResult on_event(SDL_Event *event);
  ~Core();
};

#endif
