#include "game.h"

#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "input.h"
#include "scene.h"

Core *core = NULL;
Scene *mainScene = NULL;

bool quit = false;

bool initGame(GameParameters *gameParameters) {
  core = initCore(gameParameters->screenWidth, gameParameters->screenHeight, gameParameters->title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return false;
  }
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
    processInput(&quit);
    SDL_SetRenderDrawColor(core->renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(core->renderer);
    SDL_RenderPresent(core->renderer);

    renderScene(mainScene, core->renderer);

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

