#include <iostream>
#include <stdexcept>
#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <yaml-cpp/yaml.h>

#include "Core.hpp"
#include "Font.hpp"

SDL_Texture *goose = NULL;

struct AppState {
  Core *core;
  Font *font;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  
  std::cout << "Loading config..." << std::endl;
  YAML::Node config = YAML::LoadFile("config.yaml");
  YAML::Node window = config["window"];

  std::string title = window["title"].as<std::string>();
  int width = window["width"].as<int>();
  int height = window["height"].as<int>();
  std::cout << width << " x " << height << std::endl;

  try {
    Core *core = new Core(title, width, height);
    Font *font = new Font(core->renderer,
                          "assets/fonts/Tektur-Bold.ttf", 32, 4);

    SDL_Color white = {0xEE, 0xEE, 0xEE, 0xFF};
    SDL_Color black = {0x44, 0x44, 0x44, 0xFF};
    core->hello = font->render_text("Hello, World!", white, black);

    *appstate = new AppState{
        .core = core,
        .font = font,
    };
    return SDL_APP_CONTINUE;
  } catch (const std::runtime_error err) {
    std::cerr << err.what() << std::endl;
    return SDL_APP_FAILURE;
  }
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
