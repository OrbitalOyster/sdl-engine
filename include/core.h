#ifndef CORE_H
#define CORE_H

#include <SDL2/SDL.h>

// TODO Config file
#define WINDOW_POSX SDL_WINDOWPOS_CENTERED
#define WINDOW_POSY SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

typedef struct Core Core;

Core *initCore(int windowWidth, int windowHeight, char *title);
SDL_Renderer *getRenderer(Core *core);
void destroyCore(Core *core);

#endif /* CORE_H */
