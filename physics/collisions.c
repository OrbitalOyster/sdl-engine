#include "collisions.h"

#include <stdlib.h>

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
  EntityCollisionState *result = createEmptyHeapEntityCollisionState();

  for (uint32_t i = 0; i < scene->numberOfProps; i++) {
    Prop *prop = scene->props[i];

    // Ignore collisions
    if (!(entity->collisionMask & prop->collisionId))
      continue;

    OrthoRectCollision rc = getOrthoRectCollision(entity->rect, prop->rect);
    if (rc.type) {
      updateCollisionState(result, prop, rc);
    }
  }

  return result;
}

