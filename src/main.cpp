#define SDL_MAIN_USE_CALLBACKS

#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// #include <SDL3_image/SDL_image.h>

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

struct AppContext {
  Core core;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  Core core;
  *appstate = new AppContext{
    .core = core,
  };
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}
