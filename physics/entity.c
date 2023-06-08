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
  INFOF("Jumped entity #%u to (%.8f %.8f)", entity->tag, entity->x, entity->y);
}

void setEntityVelocity(Entity *entity, double vx, double vy) {
  entity->_vx = vx;
  entity->_vy = vy;
  entity->_avx = entity->_vx;
  entity->_avy = entity->_vy;
  INFOF("Set entity #%u velocity to (%.8f %.8f)", entity->tag, entity->_vx, entity->_vy);
}

void increaseEntityVelocity(Entity *entity, double dvx, double dvy) {
  entity->_vx += dvx;
  entity->_vy += dvy;
  entity->_avx = entity->_vx;
  entity->_avy = entity->_vy;
  INFOF("Set entity #%u velocity to (%.8f %.8f)", entity->tag, entity->_vx, entity->_vy);
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

