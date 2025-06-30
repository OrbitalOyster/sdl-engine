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
  SDL_RenderClear(this->renderer);

  // const SDL_FRect sourceRect = {0, 0, 512, 512};
  // const SDL_FRect destRect = {20, 30, 200, 200};
  // SDL_RenderTexture(this->renderer, this->hello, &sourceRect, &destRect);
  
  SDL_FRect dstRect = {100, 100, 316, 28};
  SDL_RenderTexture(this->renderer, this->hello, NULL, &dstRect);

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
  SDL_Texture *texture = IMG_LoadTexture(this->renderer, filename);
  if (!texture) {
    SDL_Log("Failed to load asset: %s", SDL_GetError());
    return NULL;
  }
  return texture;
}

Core::~Core() { SDL_Log("Finished"); }
