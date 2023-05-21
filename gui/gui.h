#ifndef GUI_H
#define GUI_H

#include <stdint.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "caption.h"
#include "font.h"

#define MAIN_FONT_SIZE 16
#define MAX_NUMBER_OF_CAPTIONS 1000

typedef struct {
  SDL_Renderer *renderer;
  Font *defaultFont;
  uint32_t numberOfCaptions;
  Caption **captions;
} GUI;

GUI *createGUI(SDL_Renderer *renderer);
void renderGUI(GUI *gui);
void destroyGUI(GUI *gui);

#endif
