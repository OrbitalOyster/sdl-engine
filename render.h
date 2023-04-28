#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#include "physics/prop.h"
#include "physics/entity.h"

void drawRect(SDL_Renderer* renderer, double x, double y, double w, double h);
void renderProp(SDL_Renderer* renderer, Prop* prop);
void renderEntity(SDL_Renderer *renderer, Entity *entity);

#endif
