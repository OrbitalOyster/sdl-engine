#include "caption.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../debug.h"

SDL_Texture *createCaptionTexture(SDL_Renderer *renderer, TTF_Font *font,
                                  char *text, SDL_Color *color) {
  // Create surface from font
  //SDL_Surface *tmpSurface = TTF_RenderUTF8_Blended_Wrapped(font, text, *color, 0);
  SDL_Surface *tmpSurface = TTF_RenderUTF8_Blended(font, text, *color);
  if (!tmpSurface) {
    WARNF("Unable to create surface from font: %s\n", TTF_GetError());
    return NULL;
  }
  // Convert surface to texture
  SDL_Texture *fontTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
  if (fontTexture == NULL) {
    WARNF("Unable to create texture from font: %s\n", SDL_GetError());
    return NULL;
  }
  // Free surface
  SDL_FreeSurface(tmpSurface);
  return fontTexture;
}

SDL_Texture *createOutlinedCaptionTexture(SDL_Renderer *renderer, char* text, Font* font, SDL_Color* color, SDL_Color *outlineColor, Caption *caption) {
  SDL_Texture *result;

  // Outline text on background
  SDL_Texture *bg = createCaptionTexture(renderer, font->outline,
                                         text, outlineColor);
  // Actual text on foreground
  SDL_Texture *fg = createCaptionTexture(renderer, font->ttf,
                                         text, color);
  // Get bg dimensions
  int bw;
  int bh;
  SDL_QueryTexture(bg, NULL, NULL, &bw, &bh);
  //TTF_SizeText(caption->font->ttf, caption->text, &bw, &bh);
  // ???
  caption->w = (uint32_t)bw;
  caption->h = (uint32_t)bh;
  // Get fg dimensions
  int fw;
  int fh;
  SDL_QueryTexture(fg, NULL, NULL, &fw, &fh);
  // Transparent background TODO: Make it optional bg color?
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                       SDL_TEXTUREACCESS_TARGET, bw, bh);

  SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, result);
  SDL_RenderClear(renderer);
  SDL_Rect bgRect = {0, 0, bw, bh};
  SDL_Rect fgRect = {font->outlineSize, font->outlineSize, fw,
                    fh};
  // Copy bg, fg, clear render target
  SDL_RenderCopy(renderer, bg, NULL, &bgRect);
  SDL_RenderCopy(renderer, fg, NULL, &fgRect);
  SDL_SetRenderTarget(renderer, NULL);
  // Clean up
  SDL_DestroyTexture(bg);
  SDL_DestroyTexture(fg);

  return result;
}

void updateCaptionTexture(SDL_Renderer *renderer, Caption *caption) {
  if (caption->texture)
    SDL_DestroyTexture(caption->texture);

  for (unsigned int n = 0; n < caption->lines; n++)
    SDL_DestroyTexture(caption->textures[n]);

  caption -> lines = 0;
  uint32_t n = 0;
  uint8_t ln = 0;
  char line[MAX_CAPTION_LINE_SIZE];
  char c;
  bool done = false;
  while (!done) {
    c = caption->text[n++];
    line[ln] = c;

    if (c == '\n' || c == '\0') {
      line[ln] = '\0';
      caption->textures[caption->lines] = createOutlinedCaptionTexture(renderer, line, caption->font, caption->color, caption->outlineColor, caption);
      caption->lines++;
      ln = 0;
      printf("DEBUG '%s'\n", line);
      if (c == '\0') done = true;
    }
    else ln++;
  }

  INFOF("Updating caption texture. Number of lines: %u", caption->lines);
  caption->texture = createOutlinedCaptionTexture(renderer, caption->text, caption->font, caption->color, caption->outlineColor, caption);

  //caption->texture = caption->textures[0];
}

Caption *createCaption(SDL_Renderer *renderer, uint32_t x, uint32_t y,
                       char *text, Font *font, SDL_Color *color,
                       SDL_Color *outlineColor) {
  Caption *caption = calloc(1, sizeof(Caption));
  *caption = (Caption){.x = x,
                       .y = y,
                       .text = text,
                       .lines = 0,
                       .font = font,
                       .color = color,
                       .outlineColor = outlineColor,
                       .w = 0,
                       .h = 0,
                       .texture = NULL,
                       .textures = calloc(MAX_CAPTION_LINES, sizeof(SDL_Texture*))};
  updateCaptionTexture(renderer, caption);
  return caption;
}

void destroyCaption(Caption *caption) {
  SDL_DestroyTexture(caption->texture);
  free(caption);
}

