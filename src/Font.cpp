#include "Font.hpp"
#include <stdexcept>

Font::Font(SDL_Renderer *renderer, const char *filename, float size,
           float outline_size)
    : renderer(renderer), filename(filename), size(size),
      outline_size(outline_size) {
  // TODO: Not here
  TTF_Init();

  ttf = TTF_OpenFont(filename, size);
  if (!ttf) {
    SDL_Log("Unable to open font %s\n", filename);
    SDL_Log("%s", SDL_GetError());
    // return NULL;
  }

  outline = TTF_OpenFont(filename, size);
  if (!outline) {
    SDL_Log("Unable to open font %s\n", filename);
    // return NULL;
  }
  TTF_SetFontOutline(outline, outline_size);

  line_height = 0;

  SDL_Log("Loaded font \"%s\", size %f, outline %f", filename, size,
          outline_size);
}

SDL_Texture *Font::render_text(const char *text, bool is_outline,
                               SDL_Color color) {
  // Create surface from font
  SDL_Surface *tmp_surface =
      TTF_RenderText_Blended(is_outline ? outline : ttf, text, 0, color);
  if (!tmp_surface) {
    SDL_Log("Unable to create surface from font: %s\n", SDL_GetError());
    // return NULL;
  }
  // Convert surface to texture
  SDL_Texture *font_texture =
      SDL_CreateTextureFromSurface(renderer, tmp_surface);
  if (font_texture == NULL) {
    SDL_Log("Unable to create texture from font: %s\n", SDL_GetError());
    // return NULL;
  }
  // Free tmp surface
  SDL_DestroySurface(tmp_surface);
  return font_texture;
}

SDL_Texture *Font::render_text(const char *text, SDL_Color color,
                               SDL_Color outline_color) {
  SDL_Texture *result;
  // Outline text on background
  SDL_Texture *bg = render_text(text, true, outline_color);
  // Actual text on foreground
  SDL_Texture *fg = render_text(text, false, color);
  // Get bg dimensions
  float bw;
  float bh;
  SDL_GetTextureSize(bg, &bw, &bh);
  SDL_Log("%f %f\n", bw, bh);
  //  Get fg dimensions
  float fw;
  float fh;
  SDL_GetTextureSize(fg, &fw, &fh);
  SDL_Log("%f %f\n", fw, fh);
  // Transparent background
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                             SDL_TEXTUREACCESS_TARGET, bw, bh);
  SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, result);
  SDL_RenderClear(renderer);
  const SDL_FRect bg_rect = {0, 0, bw, bh};
  const SDL_FRect fg_rect = {outline_size, outline_size, fw, fh};
  // Copy bg, fg, clear render target
  SDL_RenderTexture(renderer, bg, NULL, &bg_rect);
  SDL_RenderTexture(renderer, fg, NULL, &fg_rect);
  SDL_SetRenderTarget(renderer, NULL);
  // Clean up
  SDL_DestroyTexture(bg);
  SDL_DestroyTexture(fg);
  // Done
  return result;
}

Font::~Font() {
  TTF_CloseFont(ttf);
  TTF_CloseFont(outline);
}
