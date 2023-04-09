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
  switch (key) {
    case 20: // q
      stopGame();
      break;
    case 26: // w
      player->_vy -= .13;
      break;
    case 7: // d
      player->_vx += .13;
      break;
    case 22: // s
      player->_vy += .13;
      break;
    case 4: // a
      player->_vx -= .13;
      break;
    default:
      break;
  }
}

void onKeyUp(SDL_Scancode key) {
  INFOF("Key released: %i/%c", key, SDL_GetKeyFromScancode(key));
  switch (key) {
    case 26: // w
      player->_vy += .13;
      break;
    case 7: // d
      player->_vx -= .13;
      break;
    case 22: // s
      player->_vy -= .13;
      break;
    case 4: // a
      player->_vx += .13;
      break;
    default:
      break;
  }
}

int main() {
  GameParameters gameParameters = {.screenWidth = 640, .screenHeight = 480, .title = "Untitled" };
  if (!initGame(&gameParameters)) return 1;
  registerOnKeyDownFunc(onKeyDown);
  registerOnKeyUpFunc(onKeyUp);

  player = createEntity(120, 200 - 48, 48, 48);
  player->collisionId = 1;
  player->collisionMask = 2;

  ground = createProp(100, 200, 250, 32);
  ground->collisionId = 2;

  INFOF("Player position: %lf %lf", player->x, player->y);
  INFOF("Ground position: %lf %lf", ground->x, ground->y);

  addPropToScene(getMainScene(), ground);
  addEntityToScene(getMainScene(), player);

  startGame();
  return 0;
}

