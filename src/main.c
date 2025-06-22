#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_Texture *goose = NULL;

SDL_Texture *load_png(SDL_Renderer *renderer, char *filename) {
  SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
  if (!texture) {
    SDL_Log("Failed to load asset: %s", SDL_GetError());
    return NULL;
  }
  return texture;
}

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

  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) { SDL_Log("Finished"); }
