#include <stdio.h>

#include "core.h"
#include "input.h"
#include "utils/debug.h"
#include "utils/JSON/JSON.h"

int main() {

  struct JSON *json = create_JSON();
  parse_JSON_file(json, "sample.json");
  // parse_JSON_file(json, "test-data/large-file.json");
  char *s = JSON_to_string(json);
  printf("res = %s\n", s);

  /*
  unsigned int INITIAL_STR_LENGTH = 16;
  unsigned int l = 0;
  char *str = "Test string";

  char *result = calloc(INITIAL_STR_LENGTH, sizeof(char));
  char c = str[0];
  while (c) {
    result[l] = c;
    c = str[++l];
  }
  printf("res = %s, l = %u strlen = %lu\n", result , l, strlen("123"));
  */



  return 0;

  int quit = 0;
  Core *core = init_core(640, 480, "Untitled");
  // Something went wrong
  if (!core) {
    WARN("Unable to start engine");
    return 0;
  }

  reset_key_input();

  SDL_Renderer *renderer = get_renderer(core);
  while (!quit) {
    // Input
    process_input(&quit);
    // Render
    SDL_SetRenderDrawColor(renderer, 0x77, 0x77, 0xCC, 0xFF);
    SDL_RenderClear(renderer);
    // Done
    SDL_RenderPresent(renderer);
    // Delay
    SDL_Delay(50);
  }

  destroy_core(core);
}
