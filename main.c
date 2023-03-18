#include <stdio.h>

#include "sdl_core.h"

int main() {
  printf("Starting engine...\n");
  sdl_core_t* core = init_sdl_core();
  SDL_Delay(1000);
  close_core(core);
  printf("Engine closed\n");
  return 0;
}

