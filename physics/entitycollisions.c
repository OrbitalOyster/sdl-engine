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

  for (unsigned int i = 0; i < scene->numberOfProps; i++) {
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

EntityImmediateCollisionChange
getEntityImmediateCollisionChange(Entity *entity, double vx, double vy) {
  EntityImmediateCollisionChange result = {
      .size = 0,
      .changes = calloc(MAX_ENTITY_COLLISION_CHANGE_SIZE,
                        sizeof(EntityNextCollisionChange))};

  for (uint8_t i = 0; i < entity->collisionState->size; i++) {
    Prop *prop = entity->collisionState->collisions[i]->prop;
    uint8_t mask = getMovingOrthoRectsImmediateCollisionChange(
        entity->rect, prop->rect, vx, vy, 0, 0);
    if (mask) {
      EntityCollisionChange ncc = {.mask = mask, .prop = prop};
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

  for (unsigned int i = 0; i < scene->numberOfProps; i++) {
    Prop *prop = scene->props[i];
    // NOTE: Skipping props from collisionState leads to bugs
    double t = getMovingOrthoRectsNextCollisionTime(entity->rect, prop->rect,
                                                    vx, vy, 0, 0);
    if (lessThan(t, result))
      result = t;
  }

  return result;
}

