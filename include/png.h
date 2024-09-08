#ifndef PNG_H
#define PNG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *load_png(SDL_Renderer *renderer, char *filename);

#endif /* PNG_H */
