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
  scene->callbacks = calloc(NUMBER_OF_COLLISION_CALLBACKS, sizeof(void *));
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

// Returns true if any immediate collision changes has been found
bool adjustEntityVelocity(Entity *entity, Scene *scene) {
  entity->_avx = entity->_vx;
  entity->_avy = entity->_vy;
  EntityImmediateCollisionChange eicc =
      getEntityImmediateCollisionChange(entity, entity->_vx, entity->_vy);
  if (eicc.size) {
//    INFOF("Found %u", eicc.size);
    for (uint8_t i = 0; i < eicc.size; i++) {
      void *agent = eicc.changes[i].agent;
      uint8_t collisionId = 0;
      OrthoRect *rect = NULL;
//      INFOF("Agent type: %i", eicc.changes[i].agentType);
      switch (eicc.changes[i].agentType) {
        case CAT_PROP:
          rect = ((Prop*) agent) -> rect;
          collisionId = ((Prop*) agent) -> collisionId;
          break;
        case CAT_ENTITY:
          rect = ((Entity*) agent) -> rect;
          collisionId = ((Entity*) agent) -> collisionId;
          break;
      }
//      INFOF("Collision id: %u", collisionId);
      uint8_t mask = entity->collisionMask & collisionId;
//      INFOF("Collision mask: %u", mask);
      if (scene->callbacks[mask]) {
        physicsCallbackStats s = {.r1 = entity->rect,
                                  .r2 = rect,
                                  .vx = &(entity->_avx),
                                  .vy = &(entity->_avy),
                                  .collisionChangeMask = eicc.changes[i].mask};
        scene->callbacks[mask](s);
      }
    }
  }
  freeEntityImmediateCollisionChange(eicc);
  return eicc.size > 0;
}

/*
double stepEntity(Entity *entity, Scene *scene, double timeToProcess) {
  INFOF("Stepping entity (%.8lf, %.8lf)", entity->_vx, entity->_vy);
  INFOF("Entity collision state size: %u", entity->collisionState->size);

  // Step 1: check for immediate changes
  bool adjustedVelocity = adjustEntityVelocity(entity, scene);

  // Step 2: get time until next collision change
  double timeUntilNextCollision =
      getEntityNextCollisionTime(entity, scene, entity->_avx, entity->_avy);

  // Collision change
  if (lessEqThan(timeUntilNextCollision, timeToProcess)) {
    moveEntity(entity, entity->_avx * timeUntilNextCollision,
               entity->_avy * timeUntilNextCollision);
   entity->collisionState = getEntityCollisionState(entity, scene);
    return timeUntilNextCollision;
  }
  // End of step
  else {
    moveEntity(entity, entity->_avx * timeToProcess, entity->_avy * timeToProcess);
    if (adjustedVelocity)
      entity->collisionState = getEntityCollisionState(entity, scene);
    return timeToProcess;
  }
}

void processEntity(Entity *entity, Scene *scene, uint64_t ticksPassed) {
  // Nothing to do
  if (compare(entity->_vx, 0) && compare(entity->_vy, 0))
    return;

  double timeProcessed = (double)ticksPassed;

  uint8_t steps = 5;
  while (moreThan(timeProcessed, 0)) {
    if (!--steps)
      ERR(1, "TOO MANY STEPS");
    timeProcessed -= stepEntity(entity, scene, timeProcessed);
  }
}
*/

double stepScene(Scene *scene, double timeToProcess) {
  double timeProcessed = (double) timeToProcess;

  // Adjust velocities, get next collision
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_vx, 0) && compare(entity->_vy, 0)) continue;
    adjustEntityVelocity(entity, scene);
    double t = getEntityNextCollisionTime(entity, scene, entity->_avx, entity->_avy);
    if (t < timeProcessed) timeProcessed = t;
  }

  // Move entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_vx, 0) && compare(entity->_vy, 0)) continue;
    moveEntity(entity,
        entity->_avx * timeProcessed,
        entity->_avy * timeProcessed);
    //TODO Don't need to get collision state every time
    entity->collisionState = getEntityCollisionState(entity, scene);
  }

  return timeProcessed;
}


void processScene(Scene *scene, uint64_t ticksPassed) {
  // INFOF("Processing scene; ticksPassed: %li", ticksPassed);
  //for (unsigned int i = 0; i < scene->numberOfEntities; i++)
  //  processEntity(scene->entities[i], scene, ticksPassed);

  INFOF("Processing scene, ticksPassed: %u", ticksPassed);
  double timeProcessed = (double)ticksPassed;

  uint16_t steps = 1000;
  while (moreThan(timeProcessed, 0)) {
    if (!--steps)
      ERR(1, "TOO MANY STEPS");
    timeProcessed -= stepScene(scene, timeProcessed);
    INFOF("Time left to process: %5.10lf", timeProcessed);
  }

  INFOF("Scene processed, steps: %u", 1000 - steps);
}

void destroyScene(Scene *scene) { free(scene); }

