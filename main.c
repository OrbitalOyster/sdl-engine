#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "debug.h"
#include "input.h"
#include "physics/entity.h"
#include "physics/prop.h"
#include "game.h"

Entity *player = NULL;
Prop *ground = NULL;

void onKeyDown(SDL_Scancode key) {
  INFOF("Key pressed: %i/%c", key, SDL_GetKeyFromScancode(key));
  if (key == 20)
    stopGame();
}

int main() {
  GameParameters gameParameters = {.screenWidth = 640, .screenHeight = 480, .title = "Untitled" };
  if (!initGame(&gameParameters)) return 1;
  registerOnKeyDownFunc(onKeyDown);

  player = createEntity(100, 200, 48, 48);
  player->collisionId = 1;

  ground = createProp(10, 300, 100, 32);

  INFOF("Player position: %lf %lf", player->x, player->y);
  INFOF("Ground position: %lf %lf", ground->x, ground->y);

  startGame();
  return 0;
}

