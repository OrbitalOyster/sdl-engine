#include "scene.h"

#include <stdlib.h>

#include "binary.h"
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
  for (unsigned int i = 0; i < scene->numberOfProps; i++)
    if (scene->props[i] == prop)
      WARN("Prop already added");

  scene->props[scene->numberOfProps] = prop;
  scene->numberOfProps++;
}

void addEntityToScene(Scene *scene, Entity *entity) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    if (scene->entities[i] == entity)
      WARN("Entity already added");

  scene->entities[scene->numberOfEntities] = entity;
  scene->numberOfEntities++;
}

void initSceneCollisions(Scene *scene) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    scene->entities[i]->collisionState =
        getEntityCollisionState(scene->entities[i], scene);
}

/*
  Returns change mask that ignores same-side collisions
  for example:
  1100 1001 => 0100 0001
*/
uint8_t getCleanMask(uint8_t mask) {
  int a = mask << 4;
  int b = mask >> 4;
  uint8_t ignoreMask = (uint8_t)(mask ^ (a + b));
  return mask & ignoreMask;
}

void slideCallback(OrthoRect *r1, OrthoRect *r2, double *vx, double *vy,
          uint8_t collisionChangeMask) {

  RelativeMovementType rmt =
      getOrthoRectsRelativeMovementType(r1, r2, *vx, *vy, 0, 0);

  if (rmt != RMT_CONVERGE)
    return;

  uint8_t cleanMask = getCleanMask(collisionChangeMask);

  if (cleanMask & (32 + 128))
    *vx = 0;

  if (cleanMask & (16 + 64))
    *vy = 0;
}

double stepEntity(Entity *entity, Scene *scene, double timeToProcess) {
  double vx = entity->_vx;
  double vy = entity->_vy;

  INFOF("Stepping entity (%.8lf, %.8lf)", vx, vy);
  INFOF("Entity collision state size: %u", entity->collisionState->size);

  // Step 1: check for immediate changes
  INFO("Getting immediate collision change: ");
  EntityImmediateCollisionChange eicc =
      getEntityImmediateCollisionChange(entity, vx, vy);
  // Call entity if any changes found
  if (eicc.size) {
    INFOF("Found %u", eicc.size);
    for (uint8_t i = 0; i < eicc.size; i++) {
      Prop *prop = eicc.changes[i].prop;
      uint8_t mask = entity->collisionMask & prop->collisionId;
      INFOF("Collision mask: %u", mask);

      INFOF("Adjusted sliding velocity: %f %f => ", vx, vy);
      if (mask == 2)
        slideCallback(entity->rect, prop->rect, &vx, &vy, eicc.changes[i].mask);
      INFOF("%f %f", vx, vy);
    }
  }

  // Step 2: get time until next collision change 
  double timeUntilNextCollision = getEntityNextCollisionTime(entity, scene, vx, vy);

  // Collision change
  if (lessEqThan(timeUntilNextCollision, timeToProcess)) {
    moveEntity(entity, vx * timeUntilNextCollision, vy * timeUntilNextCollision);
    entity->collisionState = getEntityCollisionState(entity, scene);
    freeEntityImmediateCollisionChange(eicc);
    return timeUntilNextCollision;
  }
  // End of step
  else {
    moveEntity(entity, vx * timeToProcess, vy * timeToProcess);
    if (eicc.size)
      entity->collisionState = getEntityCollisionState(entity, scene);
    freeEntityImmediateCollisionChange(eicc);
    return timeToProcess;
  }
}

void processEntity(Entity *entity, Scene *scene, uint64_t ticksPassed) {
  // Nothing to do
  if (compare(entity->_vx, 0) && compare(entity->_vy, 0))
    return;

  double timeProcessed = (double) ticksPassed;

  uint8_t steps = 5;
  while (moreThan(timeProcessed, 0)) {
    if (!--steps)
      ERR(1, "TOO MANY STEPS");
    timeProcessed -= stepEntity(entity, scene, timeProcessed);
  }
}

void processScene(Scene *scene, uint64_t ticksPassed) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    processEntity(scene->entities[i], scene, ticksPassed);
}

void destroyScene(Scene *scene) { free(scene); }

