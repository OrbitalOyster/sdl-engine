#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

#include "../geometry/collisions.h"
#include "prop.h"

typedef struct {
  Prop* prop;
  OrthoRectCollision orthoRectCollision;
} EntityCollision;

typedef struct {
  uint8_t size;
  EntityCollision** collisions;
} EntityCollisionState;

#endif
