#include "gui/caption.h"

#include <stdlib.h>

#include "utils/debug.h"

SDL_Texture *create_caption_texture(SDL_Renderer *renderer, TTF_Font *font,
                                    char *text, SDL_Color *color) {
  // Create surface from font
  SDL_Surface *tmp_surface = TTF_RenderUTF8_Blended(font, text, *color);
  if (!tmp_surface) {
    WARNF("Unable to create surface from font: %s\n", TTF_GetError());
    return NULL;
  }
  // Convert surface to texture
  SDL_Texture *font_texture =
      SDL_CreateTextureFromSurface(renderer, tmp_surface);
  if (font_texture == NULL) {
    WARNF("Unable to create texture from font: %s\n", SDL_GetError());
    return NULL;
  }
  // Free tmp surface
  SDL_FreeSurface(tmp_surface);
  return font_texture;
}

SDL_Texture *create_outlined_caption_texture(SDL_Renderer *renderer, char *text,
                                             struct Font *font,
                                             SDL_Color *color,
                                             SDL_Color *outline_color) {
  SDL_Texture *result;
  // Outline text on background
  SDL_Texture *bg =
      create_caption_texture(renderer, font->outline, text, outline_color);
  // Actual text on foreground
  SDL_Texture *fg = create_caption_texture(renderer, font->ttf, text, color);
  // Get bg dimensions
  int bw;
  int bh;
  SDL_QueryTexture(bg, NULL, NULL, &bw, &bh);
  //  Get fg dimensions
  int fw;
  int fh;
  SDL_QueryTexture(fg, NULL, NULL, &fw, &fh);
  // Transparent background
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                             SDL_TEXTUREACCESS_TARGET, bw, bh);
  SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, result);
  SDL_RenderClear(renderer);
  SDL_Rect bg_rect = {0, 0, bw, bh};
  SDL_Rect fg_rect = {font->outline_size, font->outline_size, fw, fh};
  // Copy bg, fg, clear render target
  SDL_RenderCopy(renderer, bg, NULL, &bg_rect);
  SDL_RenderCopy(renderer, fg, NULL, &fg_rect);
  SDL_SetRenderTarget(renderer, NULL);
  // Clean up
  SDL_DestroyTexture(bg);
  SDL_DestroyTexture(fg);
  // Done
  return result;
}

void update_caption_texture(SDL_Renderer *renderer, struct Caption *caption) {
  for (unsigned int n = 0; n < caption->number_of_lines; n++) {
    free(caption->lines[n]);
    SDL_DestroyTexture(caption->textures[n]);
  }

  caption->number_of_lines = 0;
  unsigned int n = 0;
  unsigned int ln = 0;
  char line[MAX_CAPTION_LINE_SIZE];
  char c;
  do {
    c = caption->text[n++];
    line[ln] = c;
    if (c == '\n' || c == '\0') {
      line[ln] = '\0';
      caption->lines[caption->number_of_lines] = calloc(ln + 1U, sizeof(char));
      strcpy(caption->lines[caption->number_of_lines], line);
      caption->textures[caption->number_of_lines] =
          create_outlined_caption_texture(renderer, line, caption->font,
                                          caption->color,
                                          caption->outline_color);
      caption->number_of_lines++;
      ln = 0;
    } else
      ln++;
  } while (c != '\0');
}

struct Caption *create_caption(SDL_Renderer *renderer, int x, int y, char *text,
                               struct Font *font, SDL_Color *color,
                               SDL_Color *outline_color) {
  struct Caption *caption = calloc(1, sizeof(struct Caption));
  *caption = (struct Caption){
      .x = x,
      .y = y,
      .text = text,
      .number_of_lines = 0,
      .lines = calloc(MAX_CAPTION_LINES, sizeof(char *)),
      .font = font,
      .color = color,
      .outline_color = outline_color,
      .leading = 1,
      .textures = calloc(MAX_CAPTION_LINES, sizeof(SDL_Texture *))};
  update_caption_texture(renderer, caption);
  return caption;
}

void destroy_caption(struct Caption *caption) {
  for (unsigned int n = 0; n < caption->number_of_lines; n++) {
    free(caption->lines[n]);
    SDL_DestroyTexture(caption->textures[n]);
  }
  free(caption);
}
