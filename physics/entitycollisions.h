#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

#include "entitycollisionstate.h"
#include "entity.h"
#include "../scene.h"

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);
EntityImmediateCollisionChange getEntityImmediateCollisionChange(Entity *entity, double vx, double vy);
double getEntityNextCollisionTime(Entity *entity, Scene *scene, double vx, double vy);
/* EntityNextCollisionChange getEntityNextCollisionChange(Entity *entity, Scene *scene, double vx, double vy); */
 
#endif
