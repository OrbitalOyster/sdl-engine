#ifndef CORE_H
#define CORE_H

#include <stdint.h>

#include <SDL2/SDL.h>

// TODO Config file
// #define WINDOW_WIDTH 640
// #define WINDOW_HEIGHT 480
#define WINDOW_POSX SDL_WINDOWPOS_CENTERED
#define WINDOW_POSY SDL_WINDOWPOS_CENTERED
#define WINDOW_FLAGS SDL_WINDOW_SHOWN
// #define WINDOW_TITLE "Untitled"
#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Core;

Core *initCore(int32_t windowWidth, int32_t windowHeight, char *title);
void drawRect(SDL_Renderer* renderer, double x, double y, double w, double h);
void destroyCore(Core *core);

#endif
