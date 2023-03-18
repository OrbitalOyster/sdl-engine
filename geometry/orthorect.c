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

void freeOrthoRect(OrthoRect *rect) {
  for (uint8_t i = 0; i < 4; i++) {
    free(rect->vertices[i]);
    free(rect->edges[i]->line);
    free(rect->edges[i]);
  }
  free(rect);
}

