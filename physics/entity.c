#include "entity.h"

#include <stdlib.h>

#include "../debug.h"

Entity *createEntity(double x, double y, double w, double h) {
  Entity *entity = calloc(1, sizeof(Entity));
  *entity = (Entity){.x = x, .y = y, .w = w, .h = h, ._vx = 0, ._vy = 0};
  entity->rect = createOrthoRect(x, y, w, h);
  return entity;
}

void jumpEntity(Entity *entity, double x, double y) {
  entity->x = x;
  entity->y = y;
  jumpOrthoRect(entity->rect, x, y);
  INFOF("Jumped entity to (%.8f %.8f)", entity->x, entity->y);
//  printf("Jumped rect: %.20f %.20f\n", entity->rect->x, entity->rect->y);
}

void moveEntity(Entity *entity, double dx, double dy) {
  entity->x += dx;
  entity->y += dy;
  moveOrthoRect(entity->rect, dx, dy);
  INFOF("Moved entity by (%.8f %.8f) to (%.8f %.8f)", dx, dy, entity->x, entity->y);
  //printf("Moved rect: %.20f %.20f\n", entity->rect->x, entity->rect->y);
}

void freeEntity(Entity *entity) {
  freeOrthoRect(entity->rect);
  free(entity);
}

