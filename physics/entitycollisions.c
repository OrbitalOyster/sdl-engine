#include "entitycollisions.h"

#include <math.h>
#include <stdlib.h>

#include "../dmath/dmath.h"
#include "../debug.h"

EntityCollisionState *createEmptyHeapEntityCollisionState() {
  EntityCollisionState *result = calloc(1, sizeof(EntityCollisionState));
  result->collisions =
      calloc(MAX_ENTITY_COLLISION_SIZE, sizeof(EntityCollision *));
  return result;
}

void updateCollisionState(EntityCollisionState *cs, Prop *prop,
                          OrthoRectCollision rc) {
  EntityCollision *ec = calloc(1, sizeof(EntityCollision));
  ec->prop = prop;
  ec->orthoRectCollision = rc;
  cs->collisions[cs->size] = ec;
  cs->size++;
}

EntityCollision *checkCollisionStateIncludesProp(EntityCollisionState *cs,
                                                 Prop *p) {
  for (uint8_t i = 0; i < cs->size; i++)
    if (cs->collisions[i]->prop == p)
      return cs->collisions[i];

  return NULL;
}

EntityCollisionState *getEntityCollisionState(Entity *entity, Scene *scene) {
  INFO("Getting entity collision state");
  EntityCollisionState *result = createEmptyHeapEntityCollisionState();

  for (uint32_t i = 0; i < scene->numberOfProps; i++) {
    Prop *prop = scene->props[i];

    // Ignore collisions
    if (!(entity->collisionMask & prop->collisionId))
      continue;

    OrthoRectCollision rc = getOrthoRectCollision(entity->rect, prop->rect);
    if (rc.type) {
      INFOF("Found %u collision with prop %u", rc.type, prop->tag);
      updateCollisionState(result, prop, rc);
    }
  }

  return result;
}

EntityNextCollisionChange
getEntityImmediateCollisionChange(Entity *entity, double vx, double vy) {
  EntityNextCollisionChange result = {.time = 0, .size = 0, .changes = NULL};

  // TODO: Proper memory allocation
  result.changes = calloc(10, sizeof(EntityNextCollisionChange));

  for (uint8_t i = 0; i < entity->collisionState->size; i++) {
    Prop *prop = entity->collisionState->collisions[i]->prop;
    uint8_t mask = getMovingOrthoRectsImmediateCollisionChange(
        entity->rect, prop->rect, vx, vy, 0, 0);
    if (mask) {
      EntityCollisionChange ncc = {.mask = mask,
                                   .prop = prop};
      result.changes[result.size++] = ncc;
    }
  }

  return result;
}

EntityNextCollisionChange getEntityNextCollisionChange(Entity *entity, Scene *scene,
                                                 double vx, double vy) {
  EntityNextCollisionChange result = {
      .time = INFINITY, .size = 0, .changes = NULL};

  // TODO: Proper memory allocation
  result.changes = calloc(10, sizeof(EntityCollisionChange));

  for (uint32_t i = 0; i < scene->numberOfProps; i++) {
    Prop *prop = scene->props[i];
    OrthoRectCollisionChange cc = getMovingOrthoRectsNextCollisionChange(
        entity->rect, prop->rect, vx, vy, 0, 0);
    // Skip
    if (compare(cc.time, INFINITY) || moreThan(cc.time,result.time)) {
      continue;
    }
    EntityCollisionChange ncc = {.mask = cc.mask, .prop = prop};
    // New candidate
    if (cc.time < result.time) {
      free(result.changes);
      result.time = cc.time;
      result.size = 1;
      result.changes = calloc(10, sizeof(EntityCollisionChange));
      result.changes[0] = ncc;
    } else result.changes[result.size++] = ncc;
  }

  return result;
}


