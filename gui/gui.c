#include "gui.h"

#include <stdlib.h>

#include <stdio.h>

GUI *createGUI(SDL_Renderer *renderer) {
  GUI *gui = calloc(1, sizeof(GUI));
  gui->renderer = renderer;

  // SDL_ttf
  if (TTF_Init() == -1) {
    printf("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
    return NULL;
  }

  gui->defaultFont = createFont("fonts/PressStart2P-Regular.ttf", 16, 3);

  gui->numberOfCaptions = 0;
  gui->captions = calloc(MAX_NUMBER_OF_CAPTIONS, sizeof(Caption *));
  return gui;
}

void renderGUI(GUI *gui) {
  // Captions
  for (unsigned int i = 0; i < gui->numberOfCaptions; i++) {
    SDL_Rect dstRect = {(int)gui->captions[i]->x, (int)gui->captions[i]->y,
                        (int)gui->captions[i]->w, (int)gui->captions[i]->h};
    SDL_RenderCopy(gui->renderer, gui->captions[i]->texture, NULL, &dstRect);
  }
}

void destroyGUI(GUI *gui) {
  // Captions
  for (unsigned int i = 0; i < gui->numberOfCaptions; i++)
    destroyCaption(gui->captions[i]);
  free(gui->captions);

  // Fonts
  destroyFont(gui->defaultFont);
  TTF_Quit();
  free(gui);
}

