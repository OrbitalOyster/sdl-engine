#include "input.h"

int keyMap[NUMBER_OF_KEYS];

void (*onKeyDownFunc)(SDL_Scancode key) = NULL;
void (*onKeyUpFunc)(SDL_Scancode key) = NULL;

void registerOnKeyDownFunc(void (*func)()) { onKeyDownFunc = func; }
void registerOnKeyUpFunc(void (*func)()) { onKeyUpFunc = func; }

void resetKeyInput() {
  for (int i = 0; i < NUMBER_OF_KEYS; i++)
    keyMap[i] = 0;
}

int processInput(int *quit) {
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
        if (onKeyDownFunc)
          onKeyDownFunc(key);
      }
      break;
    case SDL_KEYUP:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && keyMap[key]) {
        keyMap[key] = 0;
        if (onKeyUpFunc)
          onKeyUpFunc(key);
      }
      break;
    }
  }
  return 0;
}
