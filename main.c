#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "core.h"
#include "input.h"
#include "debug.h"

bool quit = false;

void onKeyDown(SDL_Scancode key) {
  INFOF("Key pressed: %i/%c", key, SDL_GetKeyFromScancode(key));
  if (key == 20) quit = true;
}

int main() {
  printf("Starting engine...\n");

  INFO("Info test");
  WARN("Warn test");

  Core* core = initCore();

  registerOnKeyDownFunc(onKeyDown);

  while (!quit) {
    processInput(&quit);
    SDL_SetRenderDrawColor(core->renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(core->renderer);
    SDL_RenderPresent(core->renderer);
    SDL_Delay(10);
  }

  destroyCore(core);
  printf("Engine closed\n");
  return 0;
}

