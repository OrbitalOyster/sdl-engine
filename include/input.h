#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#define NUMBER_OF_KEYS SDL_NUM_SCANCODES

void register_on_key_down_func(void (*func)());
void register_on_key_up_func(void (*func)());
void reset_key_input();
int process_input(int *quit);

#endif /* INPUT_H */
