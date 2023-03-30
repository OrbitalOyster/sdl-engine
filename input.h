#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

void registerOnKeyDownFunc(void (*func)());
void registerOnKeyUpFunc(void (*func)());
bool processInput();

#endif
