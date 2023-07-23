#include "scene.h"

#include <math.h>
#include <stdlib.h>

#include "binary.h"
#include "debug.h"
#include "dmath/dmath.h"
#include "physics/entitycollisions.h"
#include "utils/qsort.h"

Scene *initScene() {
  Scene *scene = calloc(1, sizeof(Scene));
  scene->props = calloc(MAX_NUMBER_OF_PROPS, sizeof(Prop *));
  scene->entities = calloc(MAX_NUMBER_OF_ENTITIES, sizeof(Entity *));
  scene->collisionTrackerSize = 0;
  scene->timeToNextCollisionChange = INFINITY;
  scene->physicsCallbacks =
      calloc(NUMBER_OF_COLLISION_CALLBACKS, sizeof(PhysicsCallback *));
  return scene;
}

void addPropToScene(Scene *scene, Prop *prop) {
  for (unsigned int i = 0; i < scene->numberOfProps; i++)
    if (scene->props[i] == prop)
      WARN("Prop already added");

  scene->props[scene->numberOfProps] = prop;
  scene->numberOfProps++;
}

bool comparePhysicsCallbacks(void **arr, int i1, int i2) {
  physicsCallbackStats *s1 = (physicsCallbackStats *)arr[i1];
  physicsCallbackStats *s2 = (physicsCallbackStats *)arr[i2];
  PhysicsCallback *c1 = s1->callback;
  PhysicsCallback *c2 = s2->callback;
  uint8_t m1 = s1->collisionChangeMask;
  uint8_t m2 = s2->collisionChangeMask;
  uint8_t p1 = (uint8_t)(c1->priority + isCornerCollisionMask(m1) * 100);
  uint8_t p2 = (uint8_t)(c2->priority + isCornerCollisionMask(m2) * 100);
  return p1 < p2;
}

bool resolveEntityVelocity(Entity *entity, Scene *scene) {
  bool result = false;
  // TODO: No magic numbers
  physicsCallbackStats **callbackStats =
      calloc(10, sizeof(physicsCallbackStats *));
  uint8_t numberOfCallbacks = 0;

  // Update collisionState
  freeEntityCollisionState(entity);
  entity->collisionState = getEntityCollisionState(entity, scene);
  // Get immediate collision change
  EntityImmediateCollisionChange eicc =
      getEntityImmediateCollisionChange(entity, entity->_avx, entity->_avy);

  INFOF("Adjusting entity #%u (cid: %s cmask: %s) velocity, collision changes: "
        "%u", entity->tag, intToBinary(entity->collisionId, 8), intToBinary(entity->collisionMask, 8), eicc.size);
  for (uint8_t i = 0; i < eicc.size; i++) {
    void *agent = eicc.changes[i].agent;
    uint8_t agentCollisionId = 0;
    OrthoRect *agentRect = NULL;
    double agentVx = 0, agentVy = 0;
    uint16_t agentTag = 0;
    switch (eicc.changes[i].agentType) {
    case CAT_PROP:
      agentRect = ((Prop *)agent)->rect;
      agentCollisionId = ((Prop *)agent)->collisionId;
      agentTag = ((Prop *)agent)->tag;
      break;
    case CAT_ENTITY:
      agentRect = ((Entity *)agent)->rect;
      agentCollisionId = ((Entity *)agent)->collisionId;
      agentVx = ((Entity *)agent)->_avx;
      agentVy = ((Entity *)agent)->_avy;
      agentTag = ((Entity *)agent)->tag;
      break;
    }
    uint8_t mask = entity->collisionMask & agentCollisionId;
    INFOF("Found %s collision with agent #%u", intToBinary(mask, 8), agentTag);
    if (scene->physicsCallbacks[mask]) {
      callbackStats[numberOfCallbacks] =
          calloc(1, sizeof(physicsCallbackStats));
      *callbackStats[numberOfCallbacks++] =
          (physicsCallbackStats){.r1 = entity->rect,
                                 .r2 = agentRect,
                                 .vx1 = &entity->_avx,
                                 .vy1 = &entity->_avy,
                                 .vx2 = agentVx,
                                 .vy2 = agentVy,
                                 .collisionChangeMask = eicc.changes[i].mask,
                                 .callback = scene->physicsCallbacks[mask]};
    } else
      INFO("No callback, skip");
  }
  // Sort callbacks by priority
  sort((void **)callbackStats, 0, numberOfCallbacks - 1,
       comparePhysicsCallbacks);
  // Fire callbacks
  INFOF("Number of callbacks: %u", numberOfCallbacks);
  for (uint8_t i = 0; i < numberOfCallbacks; i++) {
    INFOF("Firing callback %s",
          intToBinary(callbackStats[i]->collisionChangeMask, 8));
    double avx = *callbackStats[i]->vx1, avy = *callbackStats[i]->vy1;
    callbackStats[i]->callback->func(*callbackStats[i]);

    if (!compare(*callbackStats[i]->vx1, avx) ||
        !compare(*callbackStats[i]->vy1, avy)) {
      INFOF("%lf != %lf || %lf != %lf", *callbackStats[i]->vx1, avx,
            *callbackStats[i]->vy1, avy);
      result = true;
    }

    free(callbackStats[i]);
  }

  free(callbackStats);
  return result;
}

void resetSceneVelocities(Scene *scene) {
  INFO("Resetting scene velocities");
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    scene->entities[i]->_avx = scene->entities[i]->_vx;
    scene->entities[i]->_avy = scene->entities[i]->_vy;
  }
}

void resolveSceneImmediateCollisions(Scene *scene) {
  INFOF("Adjusting scene velocities (tracker size: %u)",
        scene->collisionTrackerSize);
  resetSceneVelocities(scene);
  unsigned int step = 0;
  bool vchanged = false;
  // TODO: No magic numbers
  do {
    vchanged = false;
    for (unsigned int i = 0; i < scene->collisionTrackerSize; i++)
      // Adjust velocity
      if (resolveEntityVelocity(scene->collisionTracker[i], scene)) {
        INFOF("Entity %u changed velocity", scene->collisionTracker[i]->tag);
        vchanged = true;
      }
    INFOF("Step #%u done (%i), tracker size: %u", step, vchanged,
          scene->collisionTrackerSize);
    //TODO: No magic numbers
  } while (vchanged && step++ < 10);

  if (step == 9)
    ERR(1, "9 steps!");
}

void setSceneNextCollisionTime(Scene *scene) {
  INFO("Setting next scene collision time");
  scene->timeToNextCollisionChange = INFINITY;
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    double t = getEntityNextCollisionTime(entity, scene);
    if (lessThan(t, scene->timeToNextCollisionChange))
      scene->timeToNextCollisionChange = t;
  }
  INFOF("Scene next collision change in %lfms",
        scene->timeToNextCollisionChange, scene->collisionTrackerSize);
}

// Recalculate all collisions (in brutal fashion)
// TODO: Optimization
void resetSceneCollisionTracker(Scene *scene) {
  INFO("Resetting scene collision tracker");
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    scene->collisionTracker[i] = scene->entities[i];
  scene->collisionTrackerSize = scene->numberOfEntities;
  resolveSceneImmediateCollisions(scene);
  setSceneNextCollisionTime(scene);
}

void initSceneCollisions(Scene *scene) {
  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    scene->entities[i]->collisionState =
        calloc(1, sizeof(EntityCollisionState));
  resetSceneCollisionTracker(scene);
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
  jumpOrthoRect(entity->rect, x, y);
  resetSceneCollisionTracker(scene);
  INFOF("Jumped entity #%u to (%.8f %.8f)", entity->tag, entity->x, entity->y);
}

void setEntityVelocity(Entity *entity, double vx, double vy, Scene *scene) {
  entity->_vx = vx;
  entity->_vy = vy;
  if (scene){};
  // resetSceneCollisionTracker(scene);
  INFOF("Set entity #%u velocity to (%.8f %.8f)", entity->tag, entity->_vx,
        entity->_vy);
}

void increaseEntityVelocity(Entity *entity, double dvx, double dvy,
                            Scene *scene) {
  entity->_vx += dvx;
  entity->_vy += dvy;
  resetSceneCollisionTracker(scene);
  INFOF("Set entity #%u velocity to (%.8f %.8f)", entity->tag, entity->_vx,
        entity->_vy);
}

void moveEntity(Entity *entity, double dx, double dy) {
  entity->x += dx;
  entity->y += dy;
  moveOrthoRect(entity->rect, dx, dy);
  INFOF("Moved entity #%u by (%.8f %.8f) to (%.8f %.8f)", entity->tag, dx, dy,
        entity->x, entity->y);
}

// New, improved bollocks
void resolveFoo(Scene *scene) {
  // Reset velocities
  resetSceneVelocities(scene);

  // TODO: No magic numbers
  physicsCallbackStats **callbackStats =
      calloc(10, sizeof(physicsCallbackStats *));

  uint8_t numberOfCallbacks;
  unsigned int steps = 0;

  back:
  steps++;
  numberOfCallbacks = 0;

  // Get all callbacks
  for (unsigned int j = 0; j < scene->numberOfEntities; j++) {
    Entity* entity = scene->entities[j];
    // Update collisionState
    freeEntityCollisionState(entity);
    entity->collisionState = getEntityCollisionState(entity, scene);

  // Get immediate collision change
  EntityImmediateCollisionChange eicc =
      getEntityImmediateCollisionChange(entity, entity->_avx, entity->_avy);

    INFOF("Getting entity #%u (cid: %s cmask: %s) callbacks, collision changes: "
          "%u", entity->tag, intToBinary(entity->collisionId, 8), intToBinary(entity->collisionMask, 8), eicc.size);
    for (uint8_t i = 0; i < eicc.size; i++) {
      void *agent = eicc.changes[i].agent;
      uint8_t agentCollisionId = 0;
      OrthoRect *agentRect = NULL;
      double agentVx = 0, agentVy = 0;
      uint16_t agentTag = 0;
      switch (eicc.changes[i].agentType) {
      case CAT_PROP:
        agentRect = ((Prop *)agent)->rect;
        agentCollisionId = ((Prop *)agent)->collisionId;
        agentTag = ((Prop *)agent)->tag;
        break;
      case CAT_ENTITY:
        agentRect = ((Entity *)agent)->rect;
        agentCollisionId = ((Entity *)agent)->collisionId;
        agentVx = ((Entity *)agent)->_avx;
        agentVy = ((Entity *)agent)->_avy;
        agentTag = ((Entity *)agent)->tag;
        break;
      }
      uint8_t mask = entity->collisionMask & agentCollisionId;
      INFOF("Found %s collision with agent #%u", intToBinary(mask, 8), agentTag);
      if (scene->physicsCallbacks[mask]) {
        callbackStats[numberOfCallbacks] =
            calloc(1, sizeof(physicsCallbackStats));
        *callbackStats[numberOfCallbacks++] =
            (physicsCallbackStats){.r1 = entity->rect,
                                   .r2 = agentRect,
                                   .vx1 = &entity->_avx,
                                   .vy1 = &entity->_avy,
                                   .vx2 = agentVx,
                                   .vy2 = agentVy,
                                   .collisionChangeMask = eicc.changes[i].mask,
                                   .callback = scene->physicsCallbacks[mask]};
      } else
        INFO("No callback, skip");
    }
  }


  // No callbacks - done
  if (numberOfCallbacks == 0) return;

  WARNF("Number of callbacks: %u", numberOfCallbacks);

  // Sort callbacks by priority
  sort((void **)callbackStats, 0, numberOfCallbacks - 1,
       comparePhysicsCallbacks);

  // Priority to fire
  unsigned int firstPriority = callbackStats[0]->callback->priority;

  unsigned int i = 0;
  for (; i < numberOfCallbacks; i++) {
    if (callbackStats[i]->callback->priority != firstPriority) break;
      // Fire callback
      INFOF("Firing callback %s (priority %u)",
            intToBinary(callbackStats[i]->collisionChangeMask, 8), callbackStats[i]->callback->priority );
      callbackStats[i]->callback->func(*callbackStats[i]);
      free(callbackStats[i]);
    }

  if (i < numberOfCallbacks) {
    WARN("NOT DONE!");
    if (steps > 10) ERR(1, "Too many steps");
    goto back;
  }

}

double stepScene(Scene *scene, double timeToProcess) {
  double timeProcessed = (double)timeToProcess;

  // At collision change - adjust velocities, refresh collisions
  if (compare(scene->timeToNextCollisionChange, 0)) {
    INFO("At collision change time");
    // resetSceneCollisionTracker(scene);
    resolveFoo(scene);

    // Set next collision change time
    setSceneNextCollisionTime(scene);
  }

  // Next collision is within step time range
  if (lessEqThan(scene->timeToNextCollisionChange, timeProcessed))
    timeProcessed = scene->timeToNextCollisionChange;

  // Move entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_avx, 0) && compare(entity->_avy, 0))
      continue;
    moveEntity(entity, entity->_avx * timeProcessed,
               entity->_avy * timeProcessed);
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
