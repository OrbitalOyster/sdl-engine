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
Prop *prop3 = NULL;
Entity *box = NULL;

void onKeyDown(SDL_Scancode key) {
  INFOF("Key pressed: %i/%c", key, SDL_GetKeyFromScancode(key));
  switch (key) {
    case 20: // q
      stopGame();
      break;
    case 26: // w
      increaseEntityVelocity(player, 0, -.13, getMainScene());
      //player->_vy -= .13;
      break;
    case 7: // d
      increaseEntityVelocity(player, .13, 0, getMainScene());
      //player->_vx += .13;
      break;
    case 22: // s
      increaseEntityVelocity(player, 0, .13, getMainScene());
      //player->_vy += .13;
      break;
    case 4: // a
      increaseEntityVelocity(player, -.13, 0, getMainScene());
      //player->_vx -= .13;
      break;
    // Debug section
    case 62: // F5
      debugGame(true, 0);
      break;
    case 63: // F6
      debugGame(false, 50);
      break;
    default:
      break;
  }
}

void onKeyUp(SDL_Scancode key) {
  INFOF("Key released: %i/%c", key, SDL_GetKeyFromScancode(key));
  switch (key) {
    case 26: // w
      increaseEntityVelocity(player, 0, .13, getMainScene());
      // player->_vy += .13;
      break;
    case 7: // d
      increaseEntityVelocity(player, -.13, 0, getMainScene());
      // player->_vx -= .13;
      break;
    case 22: // s
      increaseEntityVelocity(player, 0, -.13, getMainScene());
      //player->_vy -= .13;
      break;
    case 4: // a
      increaseEntityVelocity(player, .13, 0, getMainScene());
      //player->_vx += .13;
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
  INFOF("Physics slide callback mask: %s", intToBinary(s.collisionChangeMask, 8));
  INFOF("vx1: %lf, vy1: %lf, vx2: %lf, vy2: %lf", s.vx1, s.vy1, s.vx2, s.vy2);

//  RelativeMovementType rmt =
//      getOrthoRectsRelativeMovementType(s.r1, s.r2, s.vx1, s.vy1, s.vx2, s.vy2);
  RelativeFooType rft = getOrthoRectsFoo(s.r1, s.r2, s.vx1, s.vy1, s.vx2, s.vy2);

  if (rft != RFT_R1R2) {
    INFO("Not converging, skip");
    return;
  }

  uint8_t cleanMask = getCleanMask(s.collisionChangeMask);
  INFOF("Clean callback mask: %s", intToBinary(cleanMask, 8));

  // Vertical edge collision
  if (cleanMask & (32 + 128)) {
    *s.avx = 0;
    INFO("avx = 0");
  }

  // Horizontal edge collision
  if (cleanMask & (16 + 64)) {
    *s.avy = 0;
    INFO("avy = 0");
  }
}

/*
void getPushedCallback(physicsCallbackStats s) {
  INFOF("Physics getPushed callback mask: %s", intToBinary(s.collisionChangeMask, 8));
  INFOF("vx1: %lf, vy1: %lf, vx2: %lf, vy2: %lf", s.vx1, s.vy1, s.vx2, s.vy2);
}
*/

int main() {
  GameParameters gameParameters = {.screenWidth = 640, .screenHeight = 480, .title = "Untitled" };
  if (!initGame(&gameParameters)) return 1;
  registerOnKeyDownFunc(onKeyDown);
  registerOnKeyUpFunc(onKeyUp);

  player = createEntity(132, 145, 48, 48);
  //player = createEntity(340, 290, 48, 48);
  player->collisionId = 1;
  player->collisionMask = 6;
  player->tag = 0;

  ground = createProp(100, 200, 250, 32);
  ground->tag = 1;
  ground->collisionId = 2;

  prop1 = createProp(320, 142, 48, 48);
  prop1->tag = 2;
  prop1->collisionId = 2;

  prop2 = createProp(292, 290, 48, 48);
  prop2->tag = 3;
  prop2->collisionId = 2;

  prop3 = createProp(320, 142 - 48, 48, 48);
  prop3->tag = 4;
  prop3->collisionId = 2;

  box = createEntity(120, 30, 32, 32);
  box->collisionId = 4;
  box->collisionMask = 3;
  box->tag = 5;

  box->_vy = 0.1;

  addPropToScene(getMainScene(), ground);
  addPropToScene(getMainScene(), prop1);
  addPropToScene(getMainScene(), prop2);
  addPropToScene(getMainScene(), prop3);
  addEntityToScene(getMainScene(), player);
  addEntityToScene(getMainScene(), box);

  registerCollisionCallback(2, slideCallback); // Player, Box -> Prop
//  registerCollisionCallback(1, slideCallback); // Box -> Player

  startGame();
  return 0;
}

