#include "input.h"

int keyMap[NUMBER_OF_KEYS];

void (*on_key_down_func)(SDL_Scancode key) = NULL;
void (*on_key_up_func)(SDL_Scancode key) = NULL;

void register_on_key_down_func(void (*func)()) { on_key_down_func = func; }
void register_on_key_up_func(void (*func)()) { on_key_up_func = func; }

void reset_key_input() {
  for (int i = 0; i < NUMBER_OF_KEYS; i++)
    keyMap[i] = 0;
}

int process_input(int *quit) {
  SDL_Event event;
  SDL_Keycode key;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      *quit = 1;
      break;
    case SDL_KEYDOWN:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && !keyMap[key]) {
        keyMap[key] = 1;
        if (on_key_down_func)
          on_key_down_func(key);
      }
      break;
    case SDL_KEYUP:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && keyMap[key]) {
        keyMap[key] = 0;
        if (on_key_up_func)
          on_key_up_func(key);
      }
      break;
    }
  }
  return 0;
}
