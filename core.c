#include "core.h"

#include "debug.h"

Core* initCore() {
  Core* result = calloc(1, sizeof(Core));
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    ERRF(200, "Unable to initilize SDL: %s", SDL_GetError());
    return NULL;
  }
  // Init window
  result->window = SDL_CreateWindow(WINDOW_TITLE,
                                    WINDOW_POSX, WINDOW_POSY,
                                    WINDOW_WIDTH, WINDOW_HEIGHT,
                                    WINDOW_FLAGS);
  if (!result->window) {
    printf("Unable to create window: %s\n", SDL_GetError());
    return NULL;
  }
  // Init renderer
  result->renderer = SDL_CreateRenderer(result->window, -1, RENDERER_FLAGS);
  if (!result->renderer) {
    printf("Unable to create SDL renderer: %s\n", SDL_GetError());
    return NULL;
  }
  // Done
  return result;
}

void destroyCore(Core* core) {
  SDL_DestroyRenderer(core->renderer);
  core->renderer = NULL;
  SDL_DestroyWindow(core->window);
  core->window = NULL;
  SDL_Quit();
  free(core);
}

