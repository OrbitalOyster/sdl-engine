#ifndef CORE_HPP_
#define CORE_HPP_

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>

class Core {
private:
  SDL_Window *window;
  std::string title;

public:
  Core(std::string title, int window_width, int window_height);
  void iterate();
  SDL_AppResult on_event(SDL_Event *event);
  SDL_Texture *load_png(const char *filename);
  SDL_Texture *hello;
  SDL_Renderer *renderer;
  ~Core();
};

#endif
