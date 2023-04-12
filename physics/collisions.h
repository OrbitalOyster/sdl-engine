#ifndef PHYSICS_COLLISIONS_H
#define PHYSICS_COLLISIONS_H

#include "entity.h"
#include "../scene.h"
#include "entitycollisions.h"

#define MAX_ENTITY_COLLISION_SIZE 100

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);

#endif
