#ifndef CORE_HPP_
#define CORE_HPP_

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

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
