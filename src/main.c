#include <stdio.h>

#include "core.h"
#include "input.h"
#include "utils/debug.h"

int main() {
  int quit = 0;
  Core *core = initCore(640, 480, "Untitled");
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return 0;
  }
  resetKeyInput();

  SDL_Renderer *renderer = getRenderer(core);
  while (!quit) {
    // Input
    processInput(&quit);
    // Render
    SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(renderer);
    // Done
    SDL_RenderPresent(renderer);
    // Delay
    SDL_Delay(50);
  }

  destroyCore(core);
}
