#include "input.h"

#include <SDL2/SDL.h>

#include <stdio.h>

void (*onKeyDownFunc)(SDL_Scancode key) = NULL;
void (*onKeyUpFunc)(SDL_Scancode key) = NULL;

void registerOnKeyDownFunc(void (*func)()) { onKeyDownFunc = func; }
void registerOnKeyUpFunc(void (*func)()) { onKeyUpFunc = func; }

bool processInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      return true;
      break;
    case SDL_KEYDOWN:
      if (onKeyDownFunc)
        onKeyDownFunc(event.key.keysym.scancode);
      break;
    case SDL_KEYUP:
      if (onKeyUpFunc)
        onKeyUpFunc(event.key.keysym.scancode);
      break;
    }
  }
  return false;
}

