#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

#include "entitycollisionstate.h"
#include "entity.h"
#include "../scene.h"

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


EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);
EntityImmediateCollisionChange getEntityImmediateCollisionChange(Entity *entity, double vx, double vy);
double getEntityNextCollisionTime(Entity *entity, Scene *scene, double vx, double vy);
/* EntityNextCollisionChange getEntityNextCollisionChange(Entity *entity, Scene *scene, double vx, double vy); */
 
#endif
