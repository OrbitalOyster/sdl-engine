#include "input.h"

#include <SDL2/SDL.h>

bool processInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      return true;
      break;
    }
  }
  return false;
}

