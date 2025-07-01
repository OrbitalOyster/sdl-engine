#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#include "Core.hpp"
#include "Font.hpp"

SDL_Texture *goose = NULL;

struct AppState {
  Core *core;
  Font *font;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  Core *core = new Core();
  Font *font =
      new Font(core->renderer, "assets/fonts/PressStart2P-Regular.ttf", 24, 2);

  SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
  SDL_Color black = {0x00, 0x00, 0x00, 0xFF};
  core->hello = font->render_text("Hello, World!", white, black);

  *appstate = new AppState{
      .core = core,
      .font = font,
  };

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  auto a = (struct AppState *)appstate;
  return a->core->on_event(event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto a = (struct AppState *)appstate;
  a->core->iterate();
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  auto a = (struct AppState *)appstate;
  delete a->core;
}
