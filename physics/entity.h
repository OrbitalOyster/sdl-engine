#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

#include "../geometry/collisions.h"
#include "../geometry/orthorect.h"
#include "prop.h"

typedef enum {
  CAT_PROP,
  CAT_ENTITY
} CollisionAgentType;

typedef struct {
  CollisionAgentType agentType;
  void *agent;
  // Prop *prop;
  // Entity *entity;
  OrthoRectCollision orthoRectCollision;
} EntityCollision;

typedef struct {
  uint8_t size;
  EntityCollision **collisions;
} EntityCollisionState;

typedef struct {
  // Dimensions
  double x;
  double y;
  double w;
  double h;
  OrthoRect *rect;
  // "Vacuum" velocity
  double _vx;
  double _vy;
  // Actual velocity, adjusted after all collisions
  double _avx;
  double _avy;
  // Collision parameters
  uint8_t collisionId;
  uint8_t collisionMask;
  // Tag
  uint16_t tag;
  EntityCollisionState *collisionState;
} Entity;

Entity *createEntity(double x, double y, double w, double h);
void jumpEntity(Entity *entity, double x, double y);
void moveEntity(Entity *entity, double dx, double dy);

#endif
