#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "physics/prop.h"
#include "scene.h"

typedef struct {
  int32_t screenWidth;
  int32_t screenHeight;
  char *title;
} GameParameters;

bool initGame(GameParameters *gameParameters);
Scene *getMainScene();
void startGame();
void stopGame();
void destroyGame();

#endif