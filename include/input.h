#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#define NUMBER_OF_KEYS SDL_NUM_SCANCODES

void registerOnKeyDownFunc(void (*func)());
void registerOnKeyUpFunc(void (*func)());
void resetKeyInput();
int processInput(int *quit);

#endif
