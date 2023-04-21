#include "orthorect.h"

#include <stdint.h>
#include <stdlib.h>

OrthoRect *createOrthoRect(double x, double y, double w, double h) {
  OrthoRect *result = calloc(1, sizeof(OrthoRect));
  // Dimensions
  result->x = x;
  result->y = y;
  result->w = w;
  result->h = h;
  // Vertices
  result->vertices[0] = createHeapPoint(x, y);
  result->vertices[1] = createHeapPoint(x + w, y);
  result->vertices[2] = createHeapPoint(x + w, y + h);
  result->vertices[3] = createHeapPoint(x, y + h);
  // Edges
  result->edges[0] = createHeapOrthoSegment(result->vertices[0],
                                            result->vertices[1]);
  result->edges[1] = createHeapOrthoSegment(result->vertices[1],
                                            result->vertices[2]);
  result->edges[2] = createHeapOrthoSegment(result->vertices[2],
                                            result->vertices[3]);
  result->edges[3] = createHeapOrthoSegment(result->vertices[3],
                                            result->vertices[0]);
  // Done
  return result;
}

void jumpOrthoRect(OrthoRect *rect, double x, double y) {
  // Top
  rect->vertices[0]->x = x;
  rect->vertices[0]->y = y;
  jumpOrthoLine(rect->edges[0]->line, x, y);
  // Right
  rect->vertices[1]->x = x + rect->w;
  rect->vertices[1]->y = y;
  jumpOrthoLine(rect->edges[1]->line, x + rect->w, y);
  // Bottom
  rect->vertices[2]->x = x + rect->w;
  rect->vertices[2]->y = y + rect->h;
  jumpOrthoLine(rect->edges[2]->line, x + rect->w, y + rect->h);
  // Left
  rect->vertices[3]->x = x;
  rect->vertices[3]->y = y + rect->h;
  jumpOrthoLine(rect->edges[3]->line, x, y + rect->h);

  rect->x = x;
  rect->y = y;
}

void moveOrthoRect(OrthoRect *rect, double dx, double dy) {
  for (int i = 0; i < 4; i++) {
    rect->vertices[i]->x += dx;
    rect->vertices[i]->y += dy;
    moveOrthoLine(rect->edges[i]->line, dx, dy);
  }
  rect->x += dx;
  rect->y += dy;
}

void freeOrthoRect(OrthoRect *rect) {
  for (int i = 0; i < 4; i++) {
    free(rect->vertices[i]);
    freeOrthoSegment(rect -> edges[i]);
  }
  free(rect);
}

