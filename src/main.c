#include <stdio.h>

#include "core.h"
#include "input.h"
#include "utils/debug.h"

int main() {

  char *str = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
  char c = '_';

  int result = -1;
  unsigned int i1 = 0;
  unsigned int i2 = (unsigned int) strlen(str) - 1;
  unsigned int s = i2 - i1;
  unsigned int i = s / 2;

  while(1) {

    printf("i1: %u, i2: %u, s: %u, i:%u\n", i1, i2, s, i);
    printf("str[i]: %c\n", str[i]);
    printf("%c/%c [%c]\n\n", str[i1], str[i2], str[i]);

    if (str[i1] == c) {
      result = str[i1];
      break;
    }

    if (str[i2] == c) {
      result = str[i2];
      break;
    }

    if (str[i] == c) {
      result = str[i];
      break;
    }
    else {
      if (str[i] > c)
        i2 = i;
      else
        i1 = i;

      s = i2 - i1;

      if (!s) {
        break;
      }

      i = i1 + s / 2;
    }
  }

  printf("%c %i\n", result, result);

  return 0;

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
