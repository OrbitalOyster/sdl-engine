#ifndef _SDL_CORE_H_
#define _SDL_CORE_H_

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define WINDOW_POSX SDL_WINDOWPOS_CENTERED
#define WINDOW_POSY SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define WINDOW_TITLE "Untitled"
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
} sdl_core_t;

sdl_core_t* init_sdl_core();
void close_core(sdl_core_t* core);

#endif
