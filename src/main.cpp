#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdlib.h>

#include "Core.hpp"
#include "Font.hpp"

SDL_Texture *goose = NULL;

SDL_Texture *load_png(SDL_Renderer *renderer, const char *filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
  if (!texture) {
    SDL_Log("Failed to load asset: %s", SDL_GetError());
    return NULL;
  }
  return texture;
}

Font *font;

SDL_Texture *hello = NULL;

struct AppState {
  Core *core;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  *appstate = new AppState{
      .core = new Core(),
  };
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  auto a = (struct AppState *)appstate;
  return a->core->on_event(event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto a = (struct AppState *)appstate;
  a->core->iterate();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  auto a = (struct AppState *)appstate;
  delete a->core;
}
