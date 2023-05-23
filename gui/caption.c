#include "caption.h"

#include <stdlib.h>

#include "../debug.h"

SDL_Texture *createCaptionTexture(SDL_Renderer *renderer, TTF_Font *font,
                                  char *text, SDL_Color *color) {
  // Create surface from font
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

void updateCaptionTexture(SDL_Renderer *renderer, Caption *caption) {
  // Outline text on background
  SDL_Texture *bg = createCaptionTexture(renderer, caption->font->outline,
                                         caption->text, caption->outlineColor);
  // Actual text on foreground
  SDL_Texture *fg = createCaptionTexture(renderer, caption->font->ttf,
                                         caption->text, caption->color);
  // Get bg dimensions
  int bw;
  int bh;
  SDL_QueryTexture(bg, NULL, NULL, &bw, &bh);
  caption->w = (uint32_t)bw;
  caption->h = (uint32_t)bh;
  // Get fg dimensions
  int fw;
  int fh;
  SDL_QueryTexture(fg, NULL, NULL, &fw, &fh);
  // Create caption texture
  SDL_DestroyTexture(caption->texture);
  // Transparent background
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  caption->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                       SDL_TEXTUREACCESS_TARGET, bw, bh);

  SDL_SetTextureBlendMode(caption->texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, caption->texture);
  SDL_RenderClear(renderer);
  SDL_Rect bgRect = {0, 0, bw, bh};
  SDL_Rect fgRect = {caption->font->outlineSize, caption->font->outlineSize, fw,
                    fh};
  // Copy bg, fg, clear render target
  SDL_RenderCopy(renderer, bg, NULL, &bgRect);
  SDL_RenderCopy(renderer, fg, NULL, &fgRect);
  SDL_SetRenderTarget(renderer, NULL);
  // Clean up
  SDL_DestroyTexture(bg);
  SDL_DestroyTexture(fg);
}

Caption *createCaption(SDL_Renderer *renderer, uint32_t x, uint32_t y,
                       char *text, Font *font, SDL_Color *color,
                       SDL_Color *outlineColor) {
  Caption *caption = calloc(1, sizeof(Caption));
  *caption = (Caption){.x = x,
                       .y = y,
                       .text = text,
                       .font = font,
                       .color = color,
                       .outlineColor = outlineColor,
                       .w = 0,
                       .h = 0,
                       .texture = NULL};
  updateCaptionTexture(renderer, caption);
  return caption;
}

void destroyCaption(Caption *caption) {
  SDL_DestroyTexture(caption->texture);
  free(caption);
}

