#include "entity.h"

#include <stdlib.h>

Entity *createEntity(double x, double y, double w, double h) {
  Entity *entity = calloc(1, sizeof(Entity));
  *entity = (Entity){.x = x, .y = y, .w = w, .h = h, ._vx = 0, ._vy = 0};
  entity->rect = createOrthoRect(x, y, w, h);
  return entity;
}

void freeEntity(Entity *entity) {
  freeOrthoRect(entity->rect);
  free(entity);
}

