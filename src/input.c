#include "input.h"

#include "utils/debug.h"

int key_map[NUMBER_OF_KEYS];

void (*on_key_down_func)(SDL_Scancode key) = NULL;
void (*on_key_up_func)(SDL_Scancode key) = NULL;

void register_on_key_down_func(void (*func)()) { on_key_down_func = func; }
void register_on_key_up_func(void (*func)()) { on_key_up_func = func; }

void reset_key_input() {
  for (int i = 0; i < NUMBER_OF_KEYS; i++)
    key_map[i] = 0;
}

void process_input(int *quit) {
  SDL_Event event;
  SDL_Keycode key;
  while (SDL_PollEvent(&event))
    switch (event.type) {
    case SDL_QUIT:
      *quit = 1;
      break;
    // Keyboard events
    case SDL_KEYDOWN:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && !key_map[key]) {
        key_map[key] = 1;
        if (on_key_down_func)
          on_key_down_func(key);
      }
      break;
    case SDL_KEYUP:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && key_map[key]) {
        key_map[key] = 0;
        if (on_key_up_func)
          on_key_up_func(key);
      }
      break;
    // Mouse events
    case SDL_MOUSEBUTTONDOWN: {
      int x, y;
      SDL_GetMouseState(&x, &y);
      INFO2F("Mouse button down: %i %i", x, y);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      int x, y;
      SDL_GetMouseState(&x, &y);
      INFO2F("Mouse button up: %i %i", x, y);
      break;
    }
    // Window events
    case SDL_WINDOWEVENT: {
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        INFO2F("Window resize (%i x %i)", event.window.data1,
               event.window.data2);
        break;
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        INFO2("Window size changed");
        break;
      }
      break;
    }
    }
}
