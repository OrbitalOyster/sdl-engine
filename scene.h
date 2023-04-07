#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>

#include "physics/entity.h"
#include "physics/prop.h"

#define MAX_NUMBER_OF_ENTITIES 1000
#define MAX_NUMBER_OF_PROPS 1000

typedef struct {
  uint32_t numberOfProps;
  Prop **props;
  uint32_t numberOfEntities;
  Entity **entities;
} Scene;

Scene *initScene();
void addPropToScene(Scene *scene, Prop *prop);
void addEntityToScene(Scene *scene, Entity *entity);
void processScene(Scene* scene, uint64_t ticksPassed);
void freeScene(Scene *scene);

#endif
