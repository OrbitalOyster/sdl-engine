#include "Core.hpp"
#include "Font.hpp"

#include <stdlib.h>
#include <SDL3/SDL.h>
// #include <SDL3/SDL_init.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

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
  Core core;
};

SDL_AppResult SDL_AppInit(void **appstate) {
  Core core;
  *appstate = new AppState{
      .core = core,
  };
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  auto a = (struct AppState *)appstate;
  a->core.on_event(event);
  //((struct AppState*) appstate)->core.on_event(event);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto a = (struct AppState *)appstate;
  a->core.iterate();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
