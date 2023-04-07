#include "scene.h"

#include <stdlib.h>

#include "debug.h"
#include "dmath/dmath.h"

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

void processEntity(Entity *entity, Scene *scene, uint64_t ticksPassed) {
  // Nothing to do
  if (compare(entity->_vx, 0) && compare(entity->_vy, 0)) return;

  moveEntity(entity, entity->_vx * (double) ticksPassed, entity->_vy * (double) ticksPassed);

  if (scene){};
}

void processScene(Scene* scene, uint64_t ticksPassed) {
  for (uint32_t i = 0; i < scene->numberOfEntities; i++)
    processEntity(scene->entities[i], scene, ticksPassed);
}

void destroyScene(Scene* scene) {
  free(scene);
}
