#include "game.h"

#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "input.h"

Core *core = NULL;

bool quit = false;

bool initGame(GameParameters *gameParameters) {
  core = initCore(gameParameters->screenWidth, gameParameters->screenHeight, gameParameters->title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return false;
  }

  return true;
}

void startGame() {
  while (!quit) {
    processInput(&quit);
    SDL_SetRenderDrawColor(core->renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(core->renderer);
    SDL_RenderPresent(core->renderer);
    SDL_Delay(10);
  }

  destroyGame();
}

void stopGame() {
  quit = true;
}

void destroyGame() {
  destroyCore(core);
}

