#include "scene.h"

#include <stdlib.h>

#include <math.h>

#include "binary.h"
#include "debug.h"
#include "dmath/dmath.h"
#include "physics/entitycollisions.h"

Scene *initScene() {
  Scene *scene = calloc(1, sizeof(Scene));
  scene->props = calloc(MAX_NUMBER_OF_PROPS, sizeof(Prop *));
  scene->entities = calloc(MAX_NUMBER_OF_ENTITIES, sizeof(Entity *));
  scene->fooNumber = 0;
  scene->timeToNextCollisionChange = INFINITY;
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

void adjustEntityVelocity(Entity *entity, EntityImmediateCollisionChange eicc,
                          Scene *scene) {
  entity->_avx = entity->_vx;
  entity->_avy = entity->_vy;
  // Two passes, first for non-corner collisions
  for (int j = 0; j < 2; j++) {
    for (uint8_t i = 0; i < eicc.size; i++) {
      if (isCornerCollisionMask(eicc.changes[i].mask) != j)
        continue;
      void *agent = eicc.changes[i].agent;
      uint8_t collisionId = 0;
      OrthoRect *agentRect = NULL;
      double agentVx = 0;
      double agentVy = 0;
      switch (eicc.changes[i].agentType) {
      case CAT_PROP:
        agentRect = ((Prop *)agent)->rect;
        collisionId = ((Prop *)agent)->collisionId;
        break;
      case CAT_ENTITY:
        agentRect = ((Entity *)agent)->rect;
        collisionId = ((Entity *)agent)->collisionId;
        agentVx = ((Entity *)agent)->_avx;
        agentVy = ((Entity *)agent)->_avy;
        break;
      }
      uint8_t mask = entity->collisionMask & collisionId;
      if (scene->callbacks[mask]) {
        physicsCallbackStats s = {.r1 = entity->rect,
                                  .r2 = agentRect,
                                  .vx1 = entity->_avx,
                                  .vy1 = entity->_avy,
                                  .vx2 = agentVx,
                                  .vy2 = agentVy,
                                  .avx = &entity->_avx,
                                  .avy = &entity->_avy,
                                  .collisionChangeMask = eicc.changes[i].mask};
        INFOF("DEBUG %lf %lf %s", entity->_avx, entity->_avy,
              intToBinary(eicc.changes[i].mask, 8));
        INFOF("Triggering callback for entity #%u", entity->tag);
        scene->callbacks[mask](s);
      }
    }
  }
}

void adjustSceneVelocities(Scene *scene) {
  INFO("Adjusting scene velocities");
  for (unsigned int i = 0; i < scene->fooNumber; i++) {
    Entity *entity = scene->foo[i];
    EntityImmediateCollisionChange eicc =
        getEntityImmediateCollisionChange(entity, entity->_vx, entity->_vy);
    adjustEntityVelocity(entity, eicc, scene);
  }
  scene->fooNumber = 0;
}

void setSceneNextCollisionChange(Scene *scene) {
  INFO("Setting next scene collision change");
  scene->timeToNextCollisionChange = INFINITY;
  scene->fooNumber = 0;
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    double t =
        getEntityNextCollisionTime(entity, scene, entity->_avx, entity->_avy);
    if (compare(t, scene->timeToNextCollisionChange))
      scene->foo[scene->fooNumber++] = entity;
    else if (lessThan(t, scene->timeToNextCollisionChange)) {
      scene->fooNumber = 1;
      scene->foo[0] = entity;
      scene->timeToNextCollisionChange = t;
    }
  }
  INFOF("Scene next collision change in %lf", scene->timeToNextCollisionChange);
}

void initSceneCollisions(Scene *scene) {
/*  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    scene->entities[i]->collisionState =
        getEntityCollisionState(scene->entities[i], scene);
        */
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    scene->entities[i]->collisionState =
        getEntityCollisionState(scene->entities[i], scene);
    scene->foo[i] = scene->entities[i];
  }
  scene->fooNumber = scene->numberOfEntities;
  scene->timeToNextCollisionChange = 0;
  adjustSceneVelocities(scene);
  setSceneNextCollisionChange(scene);
}

void addEntityToScene(Scene *scene, Entity *entity) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    if (scene->entities[i] == entity)
      WARN("Entity already added");
  scene->entities[scene->numberOfEntities] = entity;
  scene->numberOfEntities++;
}


/*
double stepScene(Scene *scene, double timeToProcess) {
  double timeProcessed = (double) timeToProcess;

  // Adjust velocities, get next collision
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    EntityImmediateCollisionChange eicc =
        getEntityImmediateCollisionChange(entity, entity->_avx, entity->_avy);
    if (eicc.size) {
      entity->_avx = entity->_vx;
      entity->_avy = entity->_vy;
      EntityImmediateCollisionChange tmp =
          getEntityImmediateCollisionChange(entity, entity->_vx, entity->_vy);
      adjustEntityVelocity(entity, tmp, scene);
    }
    double t =
        getEntityNextCollisionTime(entity, scene, entity->_avx, entity->_avy);

    // Remember entity to have collision change next step
    if (lessThan(t, timeProcessed))
      timeProcessed = t;
  }

  // Move entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_avx, 0) && compare(entity->_avy, 0))
      continue;
    moveEntity(entity, entity->_avx * timeProcessed,
               entity->_avy * timeProcessed);
    // TODO Don't need to get collision state every time
    freeEntityCollisionState(entity);
    entity->collisionState = getEntityCollisionState(entity, scene);
  }

  return timeProcessed;
}
*/

double stepScene(Scene *scene, double timeToProcess) {
  double timeProcessed = (double) timeToProcess;
  
  if (compare(scene->timeToNextCollisionChange, 0)) {
    adjustSceneVelocities(scene);
    setSceneNextCollisionChange(scene);
  }

  if (lessEqThan(scene->timeToNextCollisionChange, timeProcessed))
    timeProcessed = scene->timeToNextCollisionChange;

  // Move entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_avx, 0) && compare(entity->_avy, 0))
      continue;
    moveEntity(entity, entity->_avx * timeProcessed,
               entity->_avy * timeProcessed);
    // TODO Don't need to get collision state every time
    freeEntityCollisionState(entity);
    entity->collisionState = getEntityCollisionState(entity, scene);
  }

  scene->timeToNextCollisionChange -= timeProcessed;
  return timeProcessed;
}

void processScene(Scene *scene, uint64_t ticksPassed) {
  INFOF("Processing scene, ticksPassed: %u", ticksPassed);
  double timeToProcess = (double)ticksPassed;

  uint16_t steps = 1000;
  while (moreThan(timeToProcess, 0)) {
    if (!--steps)
      ERR(1, "TOO MANY STEPS");
    timeToProcess -= stepScene(scene, timeToProcess);
    INFOF("Time left to process: %5.10lf", timeToProcess);
  }

  INFOF("Scene processed, steps: %u", 1000 - steps);
}

void destroyScene(Scene *scene) { free(scene); }

