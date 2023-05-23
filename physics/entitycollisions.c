#include "entitycollisions.h"

#include <math.h>
#include <stdlib.h>

#include "../debug.h"
#include "../dmath/dmath.h"

EntityCollisionState *createEmptyHeapEntityCollisionState() {
  EntityCollisionState *result = calloc(1, sizeof(EntityCollisionState));
  result->collisions =
      calloc(MAX_ENTITY_COLLISION_SIZE, sizeof(EntityCollision *));
  return result;
}

void updateCollisionState(EntityCollisionState *cs, CollisionAgentType agentType, void *agent, OrthoRectCollision rc) {
  EntityCollision *ec = calloc(1, sizeof(EntityCollision));
  ec->agentType = agentType;
  ec->agent = agent;
  ec->orthoRectCollision = rc;
  cs->collisions[cs->size] = ec;
  cs->size++;
}

EntityCollision *checkCollisionStateIncludesProp(EntityCollisionState *cs,
                                                 Prop *p) {
  for (uint8_t i = 0; i < cs->size; i++)
    if (cs->collisions[i]->agentType == CAT_PROP && cs->collisions[i]->agent == p)
      return cs->collisions[i];

  return NULL;
}

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene) {
  INFOF("Getting entity #%u collision state", entity->tag);
  EntityCollisionState *result = createEmptyHeapEntityCollisionState();

  // Props
  for (unsigned int i = 0; i < scene->numberOfProps; i++) {
    Prop *prop = scene->props[i];

    // Ignore collisions
    if (!(entity->collisionMask & prop->collisionId))
      continue;

    OrthoRectCollision rc = getOrthoRectCollision(entity->rect, prop->rect);
    if (rc.type) {
      INFOF("Found [%u] collision with prop #%u", rc.type, prop->tag);
      updateCollisionState(result, CAT_PROP, prop, rc);
    }
  }

  // Entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity2 = scene->entities[i];

    // Ignore collisions
    if (entity == entity2 || !(entity->collisionMask & entity2->collisionId))
      continue;

    OrthoRectCollision rc = getOrthoRectCollision(entity->rect, entity2->rect);
    if (rc.type) {
      INFOF("Found [%u] collision with entity #%u", rc.type, entity2->tag);
      updateCollisionState(result, CAT_ENTITY, entity2, rc);
    }
  }

  return result;
}

EntityImmediateCollisionChange
getEntityImmediateCollisionChange(Entity *entity, double vx, double vy) {
  EntityImmediateCollisionChange result = {
      .size = 0,
      .changes = calloc(MAX_ENTITY_COLLISION_CHANGE_SIZE,
                        sizeof(EntityNextCollisionChange))};

  for (uint8_t i = 0; i < entity->collisionState->size; i++) {
    CollisionAgentType agentType = entity->collisionState->collisions[i]->agentType;
    void* agent = entity->collisionState->collisions[i]->agent;
    double avx = 0;
    double avy = 0;
    OrthoRect *rect = NULL;

    switch (agentType) {
      case CAT_PROP:
        rect = ((Prop*) agent) -> rect;
        break;
      case CAT_ENTITY:
        rect = ((Entity*) agent) -> rect;
        // TODO: _vx and _vy is wrong and temporary, should use actual velocity
        avx = ((Entity*) agent) -> _vx;
        avy = ((Entity*) agent) -> _vy;
        break;
    }

    uint8_t mask = getMovingOrthoRectsImmediateCollisionChange(
        entity->rect, rect, vx, vy, avx, avy);
    if (mask) {
      EntityCollisionChange ncc = {.mask = mask, .agentType = agentType, .agent = agent};
      result.changes[result.size++] = ncc;
      if (result.size == MAX_ENTITY_COLLISION_CHANGE_SIZE)
        ERR(1, "Too many collisions");
    }
  }

  return result;
}

void freeEntityImmediateCollisionChange(EntityImmediateCollisionChange cc) {
  free(cc.changes);
}

double getEntityNextCollisionTime(Entity *entity, Scene *scene, double vx,
                                  double vy) {
  double result = INFINITY;

  // Props
  for (unsigned int i = 0; i < scene->numberOfProps; i++) {
    Prop *prop = scene->props[i];
    // NOTE: Skipping props from collisionState leads to bugs
    double t = getMovingOrthoRectsNextCollisionTime(entity->rect, prop->rect,
                                                    vx, vy, 0, 0);
    if (lessThan(t, result))
      result = t;
  }

  // Entities
  for (unsigned int i = 0; i < scene->numberOfEntities; i++) {
    Entity *entity2 = scene->entities[i];
    if (entity == entity2) continue;
    // NOTE: Skipping props from collisionState leads to bugs
    double t = getMovingOrthoRectsNextCollisionTime(entity->rect, entity2->rect,
                                                    vx, vy, entity2->_vx, entity2->_vy);
    if (lessThan(t, result))
      result = t;
  }

  return result;
}

