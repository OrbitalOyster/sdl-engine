#include "utils/JSON/token-array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TokenArray {
  size_t size;
  struct Token **content;
};

struct TokenArray *create_token_array() {
  struct TokenArray *result = calloc(1, sizeof(struct TokenArray));
  *result = (struct TokenArray){.size = 0, .content = NULL};
  return result;
}

size_t get_token_array_size(struct TokenArray *arr) { return arr->size; }

void expand_token_array(struct TokenArray *arr, struct Token *token) {
  arr->size++;
  arr->content = realloc(arr->content, arr->size * sizeof(struct Token *));
  arr->content[arr->size - 1] = token;
}

struct Token *get_token_array_element(struct TokenArray *arr, size_t n) {
  return arr->content[n];
}

void destroy_token_array(struct TokenArray *arr) {
  for (size_t i = 0; i < arr->size; i++)
    destroy_token(arr->content[i]);
  free(arr->content);
  free(arr);
}
