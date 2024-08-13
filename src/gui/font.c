#include "gui/font.h"

#include <stdlib.h>

#include "utils/debug.h"

struct Font *create_font(char *filename, int size, int outline_size) {
  struct Font *font = calloc(1, sizeof(struct Font));
  font->filename = filename;
  font->size = size;

  font->ttf = TTF_OpenFont(filename, font->size);
  if (!font->ttf) {
    WARNF("Unable to open font %s: %s\n", filename, TTF_GetError());
    return NULL;
  }
  font->outline_size = outline_size;

  font->outline = TTF_OpenFont(filename, font->size);
  if (!font->outline) {
    WARNF("Unable to open font %s: %s\n", filename, TTF_GetError());
    return NULL;
  }
  TTF_SetFontOutline(font->outline, font->outline_size);

  font->line_height = 0;

  INFOF("Loaded font \"%s\", size %u", filename, size);
  return font;
}

void destroy_font(struct Font *font) {
  TTF_CloseFont(font->ttf);
  TTF_CloseFont(font->outline);
  free(font);
}
