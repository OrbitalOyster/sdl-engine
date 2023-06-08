#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

#include "physics/entity.h"
#include "physics/prop.h"
#include "physics/callback.h"

#define MAX_NUMBER_OF_ENTITIES 1000
#define MAX_NUMBER_OF_PROPS 1000
#define NUMBER_OF_COLLISION_CALLBACKS 65536 // 16 bit

typedef struct {
  uint32_t numberOfProps;
  Prop **props;
  uint32_t numberOfEntities;
  Entity **entities;

  // TODO: Proper name
  Entity *foo[MAX_NUMBER_OF_ENTITIES];
  uint32_t fooNumber;
  double timeToNextCollisionChange;

  void (**callbacks)(physicsCallbackStats);
} Scene;

Scene *initScene();
void addPropToScene(Scene *scene, Prop *prop);
void addEntityToScene(Scene *scene, Entity *entity);
void initSceneCollisions(Scene *scene);
void jumpEntity(Entity *entity, double x, double y, Scene *scene);
void setEntityVelocity(Entity *entity, double vx, double vy, Scene *scene);
void increaseEntityVelocity(Entity *entity, double dvx, double dvy, Scene *scene);
void moveEntity(Entity *entity, double dx, double dy);
void processScene(Scene* scene, uint64_t ticksPassed);
void freeScene(Scene *scene);

#endif
