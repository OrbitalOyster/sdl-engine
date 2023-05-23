#include "game.h"

#include <stdlib.h>

#include "core.h"
#include "debug.h"
#include "input.h"
#include "gui/gui.h"
#include "render.h"
#include "scene.h"

Core *core = NULL;
Scene *mainScene = NULL;
GUI *gui = NULL;

Caption *debugCaption = NULL;
char debugText[100];

bool quit = false;
uint64_t lastTick = 0;

SDL_Color whiteColor = {0xFF, 0xFF, 0xFF, 0xFF};
SDL_Color greyColor = {0x22, 0x22, 0x22, 0xFF};

bool initGame(GameParameters *gameParameters) {
  core = initCore(gameParameters->screenWidth, gameParameters->screenHeight, gameParameters->title);
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return false;
  }
  // GUI
  gui = createGUI(core->renderer);
  if (!gui) {
    WARN("Unable to initialize GUI");
    return false;
  }

  snprintf(debugText, 100, "Tick: %lu", lastTick);
  debugCaption = createCaption(core->renderer, 10, 10, debugText, gui->defaultFont, &whiteColor, &greyColor);
  //updateCaptionTexture(core->renderer, debugCaption);
  gui->captions[gui->numberOfCaptions] = debugCaption;
  gui->numberOfCaptions++;

  // Input
  resetKeyInput();
  mainScene = initScene();
  return true;
}

Scene *getMainScene() {
  return mainScene;
}

void registerCollisionCallback(uint16_t mask, void (*func)(physicsCallbackStats)) {
  if (mainScene->callbacks[mask] != NULL)
    WARN("Callback already taken");
  mainScene->callbacks[mask] = func;
}

void renderScene(Scene *scene, SDL_Renderer *renderer) {
  for (unsigned int i = 0; i < scene->numberOfProps; i++)
    renderProp(renderer, scene->props[i]);

  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    renderEntity(renderer, scene->entities[i]);
}

void startGame() {
  initSceneCollisions(mainScene);
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
    // GUI
    renderGUI(gui);

    updateCaptionTexture(gui->renderer, debugCaption);
    snprintf(debugText, 100, "Tick: %lu", currentTick);

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

