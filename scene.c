#include "scene.h"

#include <stdlib.h>

#include "debug.h"
#include "dmath/dmath.h"
#include "physics/entitycollisions.h"

Scene *initScene() {
  Scene *scene = calloc(1, sizeof(Scene));
  scene->props = calloc(MAX_NUMBER_OF_PROPS, sizeof(Prop *));
  scene->entities = calloc(MAX_NUMBER_OF_ENTITIES, sizeof(Entity *));
  return scene;
}

void addPropToScene(Scene *scene, Prop *prop) {
  for (uint32_t i = 0; i < scene->numberOfProps; i++)
    if (scene->props[i] == prop)
      WARN("Prop already added");

  scene->props[scene->numberOfProps] = prop;
  scene->numberOfProps++;
}

void addEntityToScene(Scene *scene, Entity *entity) {
  for (uint32_t i = 0; i < scene->numberOfEntities; i++)
    if (scene->entities[i] == entity)
      WARN("Entity already added");

  scene->entities[scene->numberOfEntities] = entity;
  scene->numberOfEntities++;
}

void initSceneCollisions(Scene *scene) {
  for (uint32_t i = 0; i < scene->numberOfEntities; i++)                          
    scene->entities[i]->collisionState = getEntityCollisionState(scene->entities[i], scene);
}

uint64_t stepEntity(Entity* entity, Scene* scene, uint64_t ticksPassed) {
  double vx = entity->_vx;
  double vy = entity->_vy;
  moveEntity(entity, vx * (double) ticksPassed, vy * (double) ticksPassed);
  if (scene){};
  return ticksPassed;
}


void processEntity(Entity *entity, Scene *scene, uint64_t ticksPassed) {
  // Nothing to do
  if (compare(entity->_vx, 0) && compare(entity->_vy, 0)) return;

  uint8_t steps = 5;
  while (ticksPassed) {
    if (!--steps)
      ERR(1, "TOO MANY STEPS");
    ticksPassed -= stepEntity(entity, scene, ticksPassed);
  }
}

void processScene(Scene* scene, uint64_t ticksPassed) {
  for (uint32_t i = 0; i < scene->numberOfEntities; i++)
    processEntity(scene->entities[i], scene, ticksPassed);
}

void destroyScene(Scene* scene) {
  free(scene);
}
