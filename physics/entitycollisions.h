#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

#include <stdint.h>

#include "../scene.h"
#include "entity.h"
#include "prop.h"

#define MAX_ENTITY_COLLISION_SIZE 100
#define MAX_ENTITY_COLLISION_CHANGE_SIZE 25

typedef struct {
  CollisionAgentType agentType;
  void *agent;
  uint8_t mask;
} EntityCollisionChange;

typedef struct {
  uint8_t size;
  EntityCollisionChange *changes;
} EntityImmediateCollisionChange;

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene);
EntityImmediateCollisionChange
getEntityImmediateCollisionChange(Entity *entity, double vx, double vy);
void freeEntityImmediateCollisionChange(EntityImmediateCollisionChange cc);
double getEntityNextCollisionTime(Entity *entity, Scene *scene);
void freeEntityCollisionState(Entity* entity);

#endif
