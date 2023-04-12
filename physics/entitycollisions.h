#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

#include "entitycollisionstate.h"
#include "entity.h"
#include "../scene.h"

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);

#endif
