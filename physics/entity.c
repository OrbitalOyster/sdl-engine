#include "entity.h"

#include <stdlib.h>

#include "../debug.h"

Entity *createEntity(double x, double y, double w, double h) {
  Entity *entity = calloc(1, sizeof(Entity));
  *entity = (Entity){.x = x, .y = y, .w = w, .h = h};
  entity->rect = createOrthoRect(x, y, w, h);
  entity->immediateCollisionChange = (EntityImmediateCollisionChange){.size = 0, .changes = NULL};
  return entity;
}

void freeEntity(Entity *entity) {
  freeOrthoRect(entity->rect);
  free(entity);
}

