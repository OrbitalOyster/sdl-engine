#ifndef PHYSICS_COLLISIONS_H
#define PHYSICS_COLLISIONS_H

#include "entity.h"
#include "prop.h"
#include "../scene.h"
#include "../geometry/collisions.h"

#define MAX_ENTITY_COLLISION_SIZE 100

typedef struct {
  Prop* prop;
  OrthoRectCollision orthoRectCollision;
} EntityCollision;

typedef struct {
  uint8_t size;
  EntityCollision** collisions;
} EntityCollisionState;

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);

#endif
