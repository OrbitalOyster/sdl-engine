#include "atlas.h"

#include "png.h"
#include "utils/JSON/JSON.h"
#include "utils/debug.h"

struct Atlas *create_atlas(char *filename) {
  struct JSON *json = file_to_JSON(filename);
//  check_JSON_token(json, String, "filename");
//  check_JSON_token(json, Array, "rects");
  struct Atlas *result = calloc(1, sizeof(struct Atlas));
//  if (get_JSON_err(json))
//    WARNF("JSON err: %s", get_JSON_err(json));
  destroy_JSON(json);
  return result;
}

void destroy_atlas(struct Atlas *atlas) { SDL_DestroyTexture(atlas->texture); }
