#define SDL_MAIN_USE_CALLBACKS

#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_Texture *goose = NULL;

SDL_Texture *load_png(SDL_Renderer *renderer, const char *filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
  if (!texture) {
    SDL_Log("Failed to load asset: %s", SDL_GetError());
    return NULL;
  }
  return texture;
}

struct Font {
  const char *filename;
  TTF_Font *ttf;
  int size;
  TTF_Font *outline;
  float outline_size;
  // Must be set dynamically with SDL_QueryTexture
  unsigned int line_height;
};

struct Font *font;

struct Font *create_font(const char *filename, int size, int outline_size) {
  struct Font *font = new struct Font;
  font->filename = filename;
  font->size = size;

  font->ttf = TTF_OpenFont(filename, font->size);
  if (!font->ttf) {
    SDL_Log("Unable to open font %s\n", filename);
    SDL_Log("%s",SDL_GetError());
    return NULL;
  }
  font->outline_size = outline_size;

  font->outline = TTF_OpenFont(filename, font->size);
  if (!font->outline) {
    SDL_Log("Unable to open font %s\n", filename);
    return NULL;
  }
  TTF_SetFontOutline(font->outline, font->outline_size);

  font->line_height = 0;

  SDL_Log("Loaded font \"%s\", size %i", filename, size);
  return font;
}

SDL_Texture *create_caption_texture(SDL_Renderer *renderer, TTF_Font *font,
                                    const char *text, SDL_Color color) {
  // Create surface from font
  SDL_Surface *tmp_surface = TTF_RenderText_Blended(font, text, 0, color);
  if (!tmp_surface) {
    SDL_Log("Unable to create surface from font: %s\n", SDL_GetError());
    return NULL;
  }
  // Convert surface to texture
  SDL_Texture *font_texture =
      SDL_CreateTextureFromSurface(renderer, tmp_surface);
  if (font_texture == NULL) {
    SDL_Log("Unable to create texture from font: %s\n", SDL_GetError());
    return NULL;
  }
  // Free tmp surface
  SDL_DestroySurface(tmp_surface);
  return font_texture;
}

SDL_Texture *create_outlined_caption_texture(SDL_Renderer *renderer, const char *text,
                                             struct Font *font,
                                             SDL_Color color,
                                             SDL_Color outline_color) {
  SDL_Texture *result;
  // Outline text on background
  SDL_Texture *bg =
      create_caption_texture(renderer, font->outline, text, outline_color);
  // Actual text on foreground
  SDL_Texture *fg = create_caption_texture(renderer, font->ttf, text, color);
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
  const SDL_FRect fg_rect = {font->outline_size, font->outline_size, fw, fh};
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

void destroy_font(struct Font *font) {
  TTF_CloseFont(font->ttf);
  TTF_CloseFont(font->outline);
  free(font);
}

SDL_Texture *hello = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Engine", 640, 480, 0, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  goose = load_png(renderer, "assets/goose.png");

  TTF_Init();
  font = create_font("assets/fonts/PressStart2P-Regular.ttf", 24, 3);
  SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
  hello = create_outlined_caption_texture(renderer, "Hello, World!", font, white, black);

  SDL_Log("Started");
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  // Esc key
  if (event->type == SDL_EVENT_KEY_DOWN && event->key.scancode == SDL_SCANCODE_ESCAPE)
    return SDL_APP_SUCCESS;

  // Quit event
  if (event->type == SDL_EVENT_QUIT)
    return SDL_APP_SUCCESS;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_SetRenderDrawColor(renderer, 0x88, 0x88, 0xCC, 0xFF);
  SDL_RenderClear(renderer);

  const SDL_FRect sourceRect = {0, 0, 512, 512};
  const SDL_FRect destRect = {20, 30, 200, 200};
  SDL_RenderTexture(renderer, goose, &sourceRect, &destRect);

  SDL_FRect dstRect = {100, 100, 318, 30};
  SDL_RenderTexture(renderer, hello, NULL, &dstRect);

  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) { SDL_Log("Finished"); }
