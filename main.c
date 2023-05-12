#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "binary.h"
#include "debug.h"
#include "dmath/dmath.h"
#include "input.h"
#include "physics/entity.h"
#include "physics/prop.h"
#include "physics/callback.h"
#include "game.h"

#include "geometry/collisions.h"

Entity *player = NULL;
Prop *ground = NULL;
Prop *prop1 = NULL;
Prop *prop2 = NULL;
Entity *box = NULL;

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

/*
  Returns change mask that ignores same-side collisions
  for example:
  1100 1001 => 0100 0001
*/
uint8_t getCleanMask(uint8_t mask) {
  int a = mask << 4;
  int b = mask >> 4;
  uint8_t ignoreMask = (uint8_t)(mask ^ (a + b));
  return mask & ignoreMask;
}

void slideCallback(physicsCallbackStats s) {

  INFOF("Physics callback mask: %u", s.collisionChangeMask);

  RelativeMovementType rmt =
      getOrthoRectsRelativeMovementType(s.r1, s.r2, *s.vx, *s.vy, 0, 0);

  if (rmt != RMT_CONVERGE)
    return;

  uint8_t cleanMask = getCleanMask(s.collisionChangeMask);

  if (cleanMask & (32 + 128))
    *s.vx = 0;

  if (cleanMask & (16 + 64))
    *s.vy = 0;
}

int main() {
  GameParameters gameParameters = {.screenWidth = 640, .screenHeight = 480, .title = "Untitled" };
  if (!initGame(&gameParameters)) return 1;
  registerOnKeyDownFunc(onKeyDown);
  registerOnKeyUpFunc(onKeyUp);

  player = createEntity(120, 200 - 48, 48, 48);
  player->collisionId = 1;
  player->collisionMask = 6;

  ground = createProp(100, 200, 250, 32);
  ground->tag = 1;
  ground->collisionId = 2;

  prop1 = createProp(320, 200 - 48 - 10, 48, 48);
  prop1->tag = 2;
  prop1->collisionId = 2;

  prop2 = createProp(350 - 48 - 10, 232 + 48 + 10, 48, 48);
  prop2->tag = 3;
  prop2->collisionId = 2;

  box = createEntity(120, 30, 32, 32);
  box->collisionId = 4;
  box->collisionMask = 3;
  box->_vy = 0.1;

  addPropToScene(getMainScene(), ground);
  addPropToScene(getMainScene(), prop1);
  addPropToScene(getMainScene(), prop2);
  addEntityToScene(getMainScene(), player);
  addEntityToScene(getMainScene(), box);

  registerCollisionCallback(2, slideCallback);
  registerCollisionCallback(1, slideCallback);

  startGame();
  return 0;
}

