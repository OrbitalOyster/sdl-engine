#include "game.h"

#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "input.h"
#include "scene.h"

Core *core = NULL;
Scene *mainScene = NULL;

bool quit = false;
uint64_t lastTick = 0;

bool initGame(GameParameters *gameParameters) {
  core = initCore(gameParameters->screenWidth, gameParameters->screenHeight, gameParameters->title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return false;
  }
  resetKeyInput();
  mainScene = initScene();
  return true;
}

Scene *getMainScene() {
  return mainScene;
}

void renderScene(Scene *scene, SDL_Renderer *renderer) {
  for (uint32_t i = 0; i < scene->numberOfProps; i++) {
    drawRect(renderer, scene->props[i]->x, scene->props[i]->y, scene->props[i]->w, scene->props[i]->h);
  }
  for (uint32_t i = 0; i < scene->numberOfEntities; i++) {
    drawRect(renderer, scene->entities[i]->x, scene->entities[i]->y, scene->entities[i]->w, scene->entities[i]->h);
  }
}

void startGame() {
  while (!quit) {
    // Input
    processInput(&quit);
    const uint64_t currentTick = SDL_GetTicks();
    const uint64_t ticksPassed = currentTick - lastTick;
    // Process
    processScene(mainScene, ticksPassed);
    lastTick = currentTick;
    // Render
    SDL_SetRenderDrawColor(core->renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(core->renderer);
    renderScene(mainScene, core->renderer);
    SDL_RenderPresent(core->renderer);
    // Delay
    SDL_Delay(50);
  }

  destroyGame();
}

void stopGame() {
  quit = true;
}

void destroyGame() {
  destroyCore(core);
}

