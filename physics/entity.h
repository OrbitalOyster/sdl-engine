#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

#include "../geometry/collisions.h"
#include "../geometry/orthorect.h"
#include "prop.h"


typedef struct {
  Prop *prop;
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
  // Velocity
  double _vx;
  double _vy;
  // Collision parameters
  uint8_t collisionId;
  uint8_t collisionMask;

  EntityCollisionState *collisionState;
} Entity;

Entity *createEntity(double x, double y, double w, double h);
void jumpEntity(Entity *entity, double x, double y);
void moveEntity(Entity *entity, double dx, double dy);

#endif
