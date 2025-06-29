#include "Font.hpp"

Font::Font(const char *filename, int size, int outline_size) {
  this->filename = filename;
  this->size = size;

  this->ttf = TTF_OpenFont(filename, size);
  if (!this->ttf) {
    SDL_Log("Unable to open font %s\n", filename);
    SDL_Log("%s", SDL_GetError());
    // return NULL;
  }
  this->outline_size = outline_size;

  this->outline = TTF_OpenFont(filename, size);
  if (!this->outline) {
    SDL_Log("Unable to open font %s\n", filename);
    //return NULL;
  }
  TTF_SetFontOutline(this->outline, this->outline_size);

  this->line_height = 0;

  SDL_Log("Loaded font \"%s\", size %i", filename, size);

  /*
  TTF_Init();
  create_font("assets/fonts/PressStart2P-Regular.ttf", 24, 3);
  SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
  hello = create_outlined_caption_texture(renderer, "Hello, World!", font,
                                          white, black);
  */
}

SDL_Texture *Font::render_text(SDL_Renderer *renderer, const char *text, SDL_Color color) {
  // Create surface from font
  SDL_Surface *tmp_surface = TTF_RenderText_Blended(this->ttf, text, 0, color);
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

SDL_Texture *Font::render_outline_text(SDL_Renderer *renderer, const char *text, SDL_Color color, SDL_Color outline_color) {
  SDL_Texture *result;
  // Outline text on background
  SDL_Texture *bg =
      this->render_text(renderer, text, outline_color);
  // Actual text on foreground
  SDL_Texture *fg = this->render_text(renderer, text, color);
  // Get bg dimensions
  float bw;
  float bh;
  SDL_GetTextureSize(bg, &bw, &bh);
  SDL_Log("%f %f\n", bw, bh);
  //  Get fg dimensions
  float fw;
  float fh;
  SDL_GetTextureSize(fg, &fw, &fh);
  // Transparent background
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                             SDL_TEXTUREACCESS_TARGET, bw, bh);
  SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, result);
  SDL_RenderClear(renderer);
  const SDL_FRect bg_rect = {0, 0, bw, bh};
  const SDL_FRect fg_rect = {this->outline_size, this->outline_size, fw, fh};
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
  TTF_CloseFont(this->ttf);
  TTF_CloseFont(this->outline);
}
