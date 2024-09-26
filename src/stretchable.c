#include "stretchable.h"

struct Stretchable *create_stretchable() {
  struct Stretchable *result = calloc(1, sizeof(struct Stretchable));
  return result;
}

struct Stretchable *create_stretchable_from_token(struct Token *token) {
  struct Stretchable *result = calloc(1, sizeof(struct Stretchable));
  result->center = (SDL_Rect){.x = read_token_number(token, "center/0"),
                              .y = read_token_number(token, "center/1"),
                              .w = read_token_number(token, "center/2"),
                              .h = read_token_number(token, "center/3")};

  result->top = (SDL_Rect){.x = read_token_number(token, "top/0"),
                           .y = read_token_number(token, "top/1"),
                           .w = read_token_number(token, "top/2"),
                           .h = read_token_number(token, "top/3")};

  result->right = (SDL_Rect){.x = read_token_number(token, "right/0"),
                             .y = read_token_number(token, "right/1"),
                             .w = read_token_number(token, "right/2"),
                             .h = read_token_number(token, "right/3")};

  result->bottom = (SDL_Rect){.x = read_token_number(token, "bottom/0"),
                              .y = read_token_number(token, "bottom/1"),
                              .w = read_token_number(token, "bottom/2"),
                              .h = read_token_number(token, "bottom/3")};

  result->left = (SDL_Rect){.x = read_token_number(token, "left/0"),
                            .y = read_token_number(token, "left/1"),
                            .w = read_token_number(token, "left/2"),
                            .h = read_token_number(token, "left/3")};

  result->top_left = (SDL_Rect){.x = read_token_number(token, "topLeft/0"),
                                .y = read_token_number(token, "topLeft/1"),
                                .w = read_token_number(token, "topLeft/2"),
                                .h = read_token_number(token, "topLeft/3")};

  result->top_right = (SDL_Rect){.x = read_token_number(token, "topRight/0"),
                                 .y = read_token_number(token, "topRight/1"),
                                 .w = read_token_number(token, "topRight/2"),
                                 .h = read_token_number(token, "topRight/3")};

  result->bottom_right =
      (SDL_Rect){.x = read_token_number(token, "bottomRight/0"),
                 .y = read_token_number(token, "bottomRight/1"),
                 .w = read_token_number(token, "bottomRight/2"),
                 .h = read_token_number(token, "bottomRight/3")};

  result->bottom_left =
      (SDL_Rect){.x = read_token_number(token, "bottomLeft/0"),
                 .y = read_token_number(token, "bottomLeft/1"),
                 .w = read_token_number(token, "bottomLeft/2"),
                 .h = read_token_number(token, "bottomLeft/3")};
  return result;
}
