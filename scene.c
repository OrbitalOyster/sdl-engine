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
void adjustEntityVelocity(Entity *entity, EntityImmediateCollisionChange eicc, Scene *scene) {
  // Two passes, first for non-corner collisions
  for (int j = 0; j < 2; j++) {
    for (uint8_t i = 0; i < eicc.size; i++) {
      if (isCornerCollisionMask(eicc.changes[i].mask) != j) continue;
      void *agent = eicc.changes[i].agent;
      uint8_t collisionId = 0;
      OrthoRect *agentRect = NULL;
      double agentVx = 0;
      double agentVy = 0;
      switch (eicc.changes[i].agentType) {
        case CAT_PROP:
          agentRect = ((Prop*) agent) -> rect;
          collisionId = ((Prop*) agent) -> collisionId;
          break;
        case CAT_ENTITY:
          agentRect = ((Entity*) agent) -> rect;
          collisionId = ((Entity*) agent) -> collisionId;
          agentVx = ((Entity*) agent) -> _avx;
          agentVy = ((Entity*) agent) -> _avy;
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
        INFOF("DEBUG %lf %lf %s", entity->_avx, entity->_avy, intToBinary(eicc.changes[i].mask, 8));
        INFOF("Triggering callback for entity #%u", entity->tag);
        scene->callbacks[mask](s);
      }
    }
  }
  //freeEntityImmediateCollisionChange(eicc);
  //return eicc;
}

double stepScene(Scene *scene, double timeToProcess) {
  double timeProcessed = (double) timeToProcess;

  EntityImmediateCollisionChange * eiccs = calloc(scene->numberOfEntities, sizeof(EntityImmediateCollisionChange));

  // Adjust velocities, get next collision
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    // if (compare(entity->_vx, 0) && compare(entity->_vy, 0)) continue;
    eiccs[i] = getEntityImmediateCollisionChange(entity, entity->_avx, entity->_avy);
    // eiccs[i] = adjustEntityVelocity(entity, scene);
    if (eiccs[i].size) {
      entity->_avx = entity->_vx;
      entity->_avy = entity->_vy;

      //freeEntityCollisionState(entity);
      //entity->collisionState = getEntityCollisionState(entity, scene);

      EntityImmediateCollisionChange tmp = getEntityImmediateCollisionChange(entity, entity->_vx, entity->_vy);
      
      //adjustEntityVelocity(entity,eiccs[i], scene);
      adjustEntityVelocity(entity,tmp, scene);
    }
    double t = getEntityNextCollisionTime(entity, scene, entity->_avx, entity->_avy);
    if (t < timeProcessed) timeProcessed = t;
  }

  // Move entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity = scene->entities[i];
    if (compare(entity->_avx, 0) && compare(entity->_avy, 0)) continue;
    moveEntity(entity,
        entity->_avx * timeProcessed,
        entity->_avy * timeProcessed);
    //TODO Don't need to get collision state every time
    freeEntityCollisionState(entity);
    entity->collisionState = getEntityCollisionState(entity, scene);
  }

  for (unsigned int i = 0; i < scene->numberOfEntities; i++)
    freeEntityImmediateCollisionChange(eiccs[i]);
  free(eiccs);

  return timeProcessed;
}

void processScene(Scene *scene, uint64_t ticksPassed) {
  INFOF("Processing scene, ticksPassed: %u", ticksPassed);
  double timeToProcess = (double) ticksPassed;

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

