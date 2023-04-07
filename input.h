#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#define NUMBER_OF_KEYS SDL_NUM_SCANCODES

void registerOnKeyDownFunc(void (*func)());
void registerOnKeyUpFunc(void (*func)());
void resetKeyInput();
bool processInput(bool *quit);

#endif
