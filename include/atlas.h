/**
 * Texture atlas
 */

#ifndef ATLAS_H
#define ATLAS_H

#include <SDL2/SDL.h>

struct Atlas {
  SDL_Texture *texture;
};

struct Atlas *create_atlas(char *filename);
void destroy_atlas(struct Atlas *atlas);

#endif
