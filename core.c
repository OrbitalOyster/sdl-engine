#include "core.h"

#include <stdlib.h>

#include "debug.h"

Core *initCore(int32_t windowWidth, int32_t windowHeight, char *title) {
  INFO("Initializing SDL");
  Core *result = calloc(1, sizeof(Core));
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    WARNF("Unable to initilize SDL: %s", SDL_GetError());
    return NULL;
  }
  // Init window
  result->window = SDL_CreateWindow(title, WINDOW_POSX, WINDOW_POSY,
                                    windowWidth, windowHeight, WINDOW_FLAGS);
  if (!result->window) {
    WARNF("Unable to create window: %s", SDL_GetError());
    return NULL;
  }
  // Init renderer
  result->renderer = SDL_CreateRenderer(result->window, -1, RENDERER_FLAGS);
  if (!result->renderer) {
    WARNF("Unable to create SDL renderer: %s", SDL_GetError());
    return NULL;
  }
  // Done
  return result;
}

void destroyCore(Core *core) {
  SDL_DestroyRenderer(core->renderer);
  core->renderer = NULL;
  SDL_DestroyWindow(core->window);
  core->window = NULL;
  SDL_Quit();
  free(core);
  INFO("SDL destroyed");
}

