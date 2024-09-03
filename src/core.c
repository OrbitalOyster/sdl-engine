#include "core.h"

#include <stdlib.h>

#include "utils/debug.h"

struct Core {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

struct Core *create_core(int window_width, int window_height, char *title) {
  INFO("Initializing SDL");
  struct Core *core = calloc(1, sizeof(struct Core));
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    WARNF("Unable to create SDL: %s", SDL_GetError());
    free(core);
    return NULL;
  }
  // Init window
  core->window = SDL_CreateWindow(title, WINDOW_POSX, WINDOW_POSY, window_width,
                                  window_height, WINDOW_FLAGS);
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

SDL_Renderer *get_renderer(struct Core *core) { return core->renderer; }
SDL_Window *get_window(struct Core *core) { return core->window; }

void destroy_core(struct Core *core) {
  SDL_DestroyRenderer(core->renderer);
  core->renderer = NULL;
  SDL_DestroyWindow(core->window);
  core->window = NULL;
  SDL_Quit();
  free(core);
  INFO("SDL destroyed");
}
