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
  INFOF("Adjusting scene velocities (%u)", scene->fooNumber);
  for (unsigned int i = 0; i < scene->fooNumber; i++) {
    Entity *entity = scene->foo[i];
    freeEntityCollisionState(entity);
    entity->collisionState = getEntityCollisionState(entity, scene);
    EntityImmediateCollisionChange eicc =
        getEntityImmediateCollisionChange(entity, entity->_vx, entity->_vy);
    adjustEntityVelocity(entity, eicc, scene);
  }
}

void setSceneNextCollisionChange(Scene *scene) {
  INFO("Setting next scene collision change");
  scene->fooNumber = 0;
  scene->timeToNextCollisionChange = INFINITY;
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

// Recalculate all collisions (in brutal fashion)
void updateSceneCollisionTracker(Scene *scene) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    scene->foo[i] = scene->entities[i];
  scene->fooNumber = scene->numberOfEntities;
  adjustSceneVelocities(scene);
  setSceneNextCollisionChange(scene);
}

void initSceneCollisions(Scene *scene) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    scene->entities[i]->collisionState = calloc(1, sizeof(EntityCollisionState));
  updateSceneCollisionTracker(scene);
}

void addEntityToScene(Scene *scene, Entity *entity) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    if (scene->entities[i] == entity)
      WARN("Entity already added");
  scene->entities[scene->numberOfEntities] = entity;
  scene->numberOfEntities++;
}

void jumpEntity(Entity *entity, double x, double y, Scene *scene) {
  entity->x = x;
  entity->y = y;
  updateSceneCollisionTracker(scene);
  jumpOrthoRect(entity->rect, x, y);
  INFOF("Jumped entity #%u to (%.8f %.8f)", entity->tag, entity->x, entity->y);
}

void setEntityVelocity(Entity *entity, double vx, double vy, Scene *scene) {
  entity->_vx = vx;
  entity->_vy = vy;
  updateSceneCollisionTracker(scene);
//  entity->_avx = entity->_vx;
//  entity->_avy = entity->_vy;
  INFOF("Set entity #%u velocity to (%.8f %.8f)", entity->tag, entity->_vx, entity->_vy);
}

void increaseEntityVelocity(Entity *entity, double dvx, double dvy, Scene *scene) {
  entity->_vx += dvx;
  entity->_vy += dvy;
  updateSceneCollisionTracker(scene);
//  entity->_avx = entity->_vx;
//  entity->_avy = entity->_vy;
  INFOF("Set entity #%u velocity to (%.8f %.8f)", entity->tag, entity->_vx, entity->_vy);
}

void moveEntity(Entity *entity, double dx, double dy) {
  entity->x += dx;
  entity->y += dy;
  moveOrthoRect(entity->rect, dx, dy);
  INFOF("Moved entity #%u by (%.8f %.8f) to (%.8f %.8f)", entity->tag, dx, dy, entity->x, entity->y);
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

  // At collision change - adjust velocities, refresh collision, next step 
  if (compare(scene->timeToNextCollisionChange, 0)) {
    updateSceneCollisionTracker(scene);
    return 0;
  }

  // Next collision within step time range
  if (lessEqThan(scene->timeToNextCollisionChange, timeProcessed))
    timeProcessed = scene->timeToNextCollisionChange;

  // Move entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_avx, 0) && compare(entity->_avy, 0))
      continue;
    moveEntity(entity, entity->_avx * timeProcessed,
               entity->_avy * timeProcessed);
/*
    for (unsigned int j = 0; j < scene->fooNumber; j++)
      if (scene->foo[j] == entity) {
        freeEntityCollisionState(entity);
        entity->collisionState = getEntityCollisionState(entity, scene);
      }
      */
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

