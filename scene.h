#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

#include "geometry/orthorect.h"
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
  void (**callbacks)(physicsCallbackStats);
} Scene;

Scene *initScene();
void addPropToScene(Scene *scene, Prop *prop);
void addEntityToScene(Scene *scene, Entity *entity);
void initSceneCollisions(Scene *scene);
void processScene(Scene* scene, uint64_t ticksPassed);
void freeScene(Scene *scene);

#endif
