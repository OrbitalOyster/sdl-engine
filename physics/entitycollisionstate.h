#ifndef ENTITY_COLLISION_STATE_H
#define ENTITY_COLLISION_STATE_H

#include <stdint.h>

#include "../geometry/collisions.h"
#include "prop.h"

#define MAX_ENTITY_COLLISION_SIZE 100

typedef struct {
  Prop* prop;
  OrthoRectCollision orthoRectCollision;
} EntityCollision;

typedef struct {
  uint8_t size;
  EntityCollision** collisions;
} EntityCollisionState;

typedef struct {
  Prop* prop;
  uint8_t mask;
} EntityCollisionChange;

typedef struct {
  uint8_t size;
  EntityCollisionChange* changes;
} EntityImmediateCollisionChange;

typedef struct {
  double time;
  uint8_t size;
  EntityCollisionChange* changes;
} EntityNextCollisionChange;

#endif
