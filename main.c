#include <stdio.h>

#include <SDL2/SDL.h>

#include "build.h"
#include "core.h"
#include "input.h"
#include "debug.h"

void onKeyDown(SDL_Scancode key) {
  INFOF("Key pressed: %i/%c", key, SDL_GetKeyFromScancode(key));
}

int main() {
  printf("Starting engine...\n");

  INFO("Info test");
  WARN("Warn test");

  Core* core = initCore();

  registerOnKeyDownFunc(onKeyDown);

  while (!processInput()) {
    SDL_SetRenderDrawColor(core->renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(core->renderer);
    SDL_RenderPresent(core->renderer);
    SDL_Delay(10);
  }

  destroyCore(core);
  printf("Engine closed\n");
  return 0;
}

