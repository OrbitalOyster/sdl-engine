#include "font.h"

#include <stdlib.h>

#include "../debug.h"

Font *createFont(char *filename, uint8_t size, uint8_t outlineSize) {
  Font *font = calloc(1, sizeof(Font));
  font->size = size;

  font->ttf = TTF_OpenFont(filename, font->size);
  if (!font->ttf) {
    WARNF("Unable to open font %s: %s\n", filename, TTF_GetError());
    return NULL;
  }
  font->outlineSize = outlineSize;

  font->outline = TTF_OpenFont(filename, font->size);
  if (!font->outline) {
    WARNF("Unable to open font %s: %s\n", filename, TTF_GetError());
    return NULL;
  }
  TTF_SetFontOutline(font->outline, font->outlineSize);

  return font;
}

void destroyFont(Font *font) {
  TTF_CloseFont(font->ttf);
  TTF_CloseFont(font->outline);
  free(font);
}

