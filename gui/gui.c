#include "gui.h"

#include <stdlib.h>

#include "../debug.h"

#define FONT_TEST_STRING "@"

void setFontLineHeight(Font *font) {
  int w, h;
  TTF_SizeUTF8(font->outline, FONT_TEST_STRING, &w, &h);
  // TODO: No magic numbers
  if (h > 255)
    WARN("Font too large")
  else
    font->lineHeight = (uint8_t)h;
  INFOF("Font \"%s\" line height set to %u", font->filename, font->lineHeight);
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
  setFontLineHeight(gui->defaultFont);

  gui->numberOfCaptions = 0;
  gui->captions = calloc(MAX_NUMBER_OF_CAPTIONS, sizeof(Caption *));
  return gui;
}

void renderGUI(GUI *gui) {
  // Captions
  for (unsigned int i = 0; i < gui->numberOfCaptions; i++) {
    for (int j = 0; j < gui->captions[i]->numberOfLines; j++) {
      int totalLineHeight = gui->captions[i]->font->lineHeight + gui->captions[i]->leading;
      SDL_Rect dstRect = {gui->captions[i]->x, gui->captions[i]->y + j * totalLineHeight,
                          0, 0};
      TTF_SizeUTF8(gui->captions[i]->font->outline, gui->captions[i]->lines[j], &dstRect.w, &dstRect.h);
      SDL_RenderCopy(gui->renderer, gui->captions[i]->textures[j], NULL, &dstRect);
    }
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

