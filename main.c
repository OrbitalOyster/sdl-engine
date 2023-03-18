#include <stdio.h>

#include "core.h"

int main() {
  printf("Starting engine...\n");
  Core* core = initCore();
  SDL_Delay(1000);
  destroyCore(core);
  printf("Engine closed\n");
  return 0;
}

