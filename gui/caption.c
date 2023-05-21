#include "caption.h"

#include <stdlib.h>

#include <stdio.h>

SDL_Texture *createCaptionTexture(SDL_Renderer *renderer, TTF_Font *font,
                                  char *text, SDL_Color *color) {
  /* Create surface from font */
  SDL_Surface *tmpSurface = TTF_RenderUTF8_Blended(font, text, *color);
  if (!tmpSurface) {
    printf("Unable to create surface from font: %s\n", TTF_GetError());
    return NULL;
  }
  /* Convert surface to texture */
  SDL_Texture *fontTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
  if (fontTexture == NULL) {
    printf("Unable to create texture from font: %s\n", SDL_GetError());
    return NULL;
  }
  /* Free surface */
  SDL_FreeSurface(tmpSurface);
  return fontTexture;
}

Caption *createCaption(SDL_Renderer *renderer, Font *font, uint32_t x,
                       uint32_t y, char *text, SDL_Color *color,
                       SDL_Color *outlineColor) {
  Caption *caption = calloc(1, sizeof(Caption));
  caption->x = x;
  caption->y = y;
  caption->text = text;

  SDL_Texture *bg = createCaptionTexture(renderer, font->outline, caption->text,
                                         outlineColor);
  SDL_Texture *fg =
      createCaptionTexture(renderer, font->ttf, caption->text, color);

  int w;
  int h;
  SDL_QueryTexture(bg, NULL, NULL, &w, &h);
  caption->width = (uint32_t)w;
  caption->height = (uint32_t)h;

  int ww;
  int hh;
  SDL_QueryTexture(fg, NULL, NULL, &ww, &hh);

  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                           SDL_TEXTUREACCESS_TARGET,
                                           w, h);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, texture);
  SDL_RenderClear(renderer);
  SDL_Rect rect1 = {0, 0, w, h};
  SDL_Rect rect2 = {font->outlineSize, font->outlineSize, ww, hh};
  SDL_RenderCopy(renderer, bg, NULL, &rect1);
  SDL_RenderCopy(renderer, fg, NULL, &rect2);
  SDL_SetRenderTarget(renderer, NULL);

  caption->texture = texture;

  return caption;
}

void destroyCaption(Caption *caption) {
  SDL_DestroyTexture(caption->texture);
  free(caption);
}

