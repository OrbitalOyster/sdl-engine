#include <stdio.h>

#include "core.h"
#include "input.h"
#include "oops.h"


int main() {
  printf("Starting engine...\n");
  Core* core = initCore();

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

