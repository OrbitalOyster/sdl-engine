#include "entity.h"

#include <stdlib.h>

#include "../debug.h"

Entity *createEntity(double x, double y, double w, double h) {
  Entity *entity = calloc(1, sizeof(Entity));
  *entity = (Entity){.x = x, .y = y, .w = w, .h = h};
  entity->rect = createOrthoRect(x, y, w, h);
  return entity;
}

void jumpEntity(Entity *entity, double x, double y) {
  entity->x = x;
  entity->y = y;
  jumpOrthoRect(entity->rect, x, y);
  INFOF("Jumped entity to (%.8f %.8f)", entity->x, entity->y);
}

void moveEntity(Entity *entity, double dx, double dy) {
  entity->x += dx;
  entity->y += dy;
  moveOrthoRect(entity->rect, dx, dy);
  INFOF("Moved entity #%u by (%.8f %.8f) to (%.8f %.8f)", entity->tag, dx, dy, entity->x, entity->y);
}

void freeEntity(Entity *entity) {
  freeOrthoRect(entity->rect);
  free(entity);
}

