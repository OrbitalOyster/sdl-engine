#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

// TODO Config file
#define WINDOW_POSX SDL_WINDOWPOS_CENTERED
#define WINDOW_POSY SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

struct Core;

struct Core *init_core(int window_width, int window_height, char *title);
SDL_Renderer *get_renderer(struct Core *core);
void destroy_core(struct Core *core);

#endif /* CORE_H */
