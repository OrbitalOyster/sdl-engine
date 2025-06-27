#include <stdexcept>
#include "Core.hpp"

Core::Core() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("Unable to initialize SDL");
  }

  if (!SDL_CreateWindowAndRenderer("Engine", 640, 480, 0, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  goose = load_png(renderer, "assets/goose.png");

  TTF_Init();
  font = create_font("assets/fonts/PressStart2P-Regular.ttf", 24, 3);
  SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
  hello = create_outlined_caption_texture(renderer, "Hello, World!", font,
                                          white, black);

  SDL_Log("Started");
  return SDL_APP_CONTINUE;
}
