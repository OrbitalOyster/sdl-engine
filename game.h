#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#include "physics/prop.h"

typedef struct {
  int32_t screenWidth;
  int32_t screenHeight;
  char *title;
} GameParameters;

bool initGame(GameParameters *gameParameters);
void startGame();
void stopGame();
void destroyGame();

#endif
