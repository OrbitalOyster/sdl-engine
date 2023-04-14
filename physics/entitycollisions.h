#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

#include "entitycollisionstate.h"
#include "entity.h"
#include "../scene.h"

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);
EntityNextCollisionChange getEntityImmediateCollisionChange(Entity *entity, double vx, double vy);

#endif
