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

/*
  Returns change mask that ignores same-side collisions
  for example:
  1100 1001 => 0100 0001
*/
uint8_t getCleanMask(uint8_t mask) {
  uint8_t a = (uint8_t) (mask << 4);
  uint8_t b = (uint8_t) (mask >> 4);
  uint8_t ignoreMask = (uint8_t) (mask ^ (a + b));
  return mask & ignoreMask;
}

void foo2(OrthoRect* r1, OrthoRect* r2, double *vx, double *vy, uint8_t collisionChangeMask) {
  INFOF("foo2 mask: %u", collisionChangeMask);
  RelativeMovementType rmt = getOrthoRectsRelativeMovementType(r1, r2, *vx, *vy, 0, 0);
  uint8_t cleanMask = getCleanMask(collisionChangeMask);
  //uint8_t cleanMask = collisionChangeMask;

  if (rmt == RMT_CONVERGE) {
    if (cleanMask & (130 + 40))
      *vx = 0;

    if (cleanMask & (65 + 20))
      *vy = 0;
  }
}

uint64_t stepEntity(Entity* entity, Scene* scene, uint64_t ticksPassed) {
  double vx = entity->_vx;
  double vy = entity->_vy;

  INFOF("Stepping entity (%.8lf, %.8lf)", vx, vy);
  INFOF("Entity collision state size: %u", entity->collisionState->size);

  // Step 1: check for immediate changes
  INFO("Getting immediate collision change: ");
  EntityNextCollisionChange eicc = getEntityImmediateCollisionChange(entity, vx, vy);
  // Call entity if any changes found
  if (eicc.size) {
    INFOF("Found %u", eicc.size);
    for (uint8_t i = 0; i < eicc.size; i++) {
      Prop* prop = eicc.changes[i].prop;
      uint8_t mask = entity->collisionMask & prop->collisionId;
      INFOF("Collision mask: %u", mask);

      INFOF("Adjusted sliding velocity: %f %f => ", vx, vy);
      if (mask == 2) foo2(entity->rect, prop->rect, &vx, &vy, eicc.changes[i].mask);
      INFOF("%f %f", vx, vy);
    }
  } 

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
