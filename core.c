#include "core.h"

#include <stdlib.h>

#include "debug.h"

Core *initCore(int32_t windowWidth, int32_t windowHeight, char *title) {
  INFO("Initializing SDL");
  Core *core = calloc(1, sizeof(Core));
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    WARNF("Unable to initilize SDL: %s", SDL_GetError());
    free(core);
    return NULL;
  }
  // Init window
  core->window = SDL_CreateWindow(title, WINDOW_POSX, WINDOW_POSY,
                                    windowWidth, windowHeight, WINDOW_FLAGS);
  if (!core->window) {
    WARNF("Unable to create window: %s", SDL_GetError());
    free(core);
    return NULL;
  }
  // Init renderer
  core->renderer = SDL_CreateRenderer(core->window, -1, RENDERER_FLAGS);
  if (!core->renderer) {
    WARNF("Unable to create SDL renderer: %s", SDL_GetError());
    return NULL;
  }
  // Done
  return core;
}

void drawRect(SDL_Renderer* renderer, double x, double y, double w, double h) {
  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
  SDL_FRect *tmp = calloc(1, sizeof(SDL_FRect));
  *tmp = (SDL_FRect) {.x = (float) x, .y = (float) y, .w = (float) w, .h = (float) h};
  SDL_RenderDrawRectF(renderer, tmp);
  free(tmp);
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

