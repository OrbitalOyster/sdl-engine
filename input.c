#include "input.h"

bool keyMap[NUMBER_OF_KEYS];

void (*onKeyDownFunc)(SDL_Scancode key) = NULL;
void (*onKeyUpFunc)(SDL_Scancode key) = NULL;

void registerOnKeyDownFunc(void (*func)()) { onKeyDownFunc = func; }
void registerOnKeyUpFunc(void (*func)()) { onKeyUpFunc = func; }

void resetKeyInput() {
  for (uint32_t i = 0; i < NUMBER_OF_KEYS; i++)
    keyMap[i] = false;
}

bool processInput(bool *quit) {
  SDL_Event event;
  SDL_Keycode key;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      *quit = true;
      break;
    case SDL_KEYDOWN:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && !keyMap[key]) {
        keyMap[key] = true;
        if (onKeyDownFunc)
          onKeyDownFunc(key);
      }
      break;
    case SDL_KEYUP:
      key = event.key.keysym.scancode;
      if (key < NUMBER_OF_KEYS && keyMap[key]) {
        keyMap[key] = false;
        if (onKeyUpFunc)
          onKeyUpFunc(key);
      }
      break;
    }
  }
  return false;
}

