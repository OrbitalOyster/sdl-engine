#include "Core.hpp"
#include <SDL3/SDL_log.h>
#include <stdexcept>

Core::Core() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("Unable to initialize SDL");
  }

  if (!SDL_CreateWindowAndRenderer("Engine", 640, 480, 0, &window, &renderer)) {
    throw std::runtime_error("Unable to initialize renderer");
  }

  // goose = load_png(renderer, "assets/goose.png");

  SDL_Log("Started");
}

void Core::iterate() {
  SDL_SetRenderDrawColor(renderer, 0x88, 0x88, 0xCC, 0xFF);
  SDL_RenderClear(renderer);
  SDL_FRect dstRect = {32, 32, 198, 50};
  SDL_RenderTexture(renderer, hello, NULL, &dstRect);

  SDL_RenderPresent(renderer);
}

SDL_AppResult Core::on_event(SDL_Event *event) {
  // Esc key
  if (event->type == SDL_EVENT_KEY_DOWN &&
      event->key.scancode == SDL_SCANCODE_ESCAPE)
    return SDL_APP_SUCCESS;

  // Quit event
  if (event->type == SDL_EVENT_QUIT)
    return SDL_APP_SUCCESS;

  return SDL_APP_CONTINUE;
}

SDL_Texture *Core::load_png(const char *filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
  if (!texture) {
    SDL_Log("Failed to load asset: %s", SDL_GetError());
    return NULL;
  }
  return texture;
}

Core::~Core() { SDL_Log("Finished"); }
