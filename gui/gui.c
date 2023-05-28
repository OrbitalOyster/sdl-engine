#include "gui.h"

#include <stdlib.h>

#include "../debug.h"

#define FONT_TEST_STRING "@"

void setFontLineHeight(GUI *gui, Font *font) {
  SDL_Color whiteColor = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Color greyColor = {0x22, 0x22, 0x22, 0xFF};
  Caption *tmpCaption = createCaption(gui->renderer, 0, 0, FONT_TEST_STRING, gui->defaultFont, &whiteColor, &greyColor);
  updateCaptionTexture(gui->renderer, tmpCaption);
  // TODO: No magic numbers
  if (tmpCaption->h > 255)
    WARN("Font too large")
  else
    font->lineHeight = (uint8_t)tmpCaption->h;
  INFOF("Font \"%s\" line height set to %u", font->filename, font->lineHeight);
  destroyCaption(tmpCaption);
}

GUI *createGUI(SDL_Renderer *renderer) {
  GUI *gui = calloc(1, sizeof(GUI));
  gui->renderer = renderer;

  // SDL_ttf
  if (TTF_Init() == -1) {
    WARNF("Unable to initialize SDL_ttf: %s\n", TTF_GetError());
    return NULL;
  }

  // TODO: Load it from somewhere
  gui->defaultFont = createFont("fonts/PressStart2P-Regular.ttf", 16, 2);
  setFontLineHeight(gui, gui->defaultFont);

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

