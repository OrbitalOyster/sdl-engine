#ifndef TOKEN_ARRAY_H
#define TOKEN_ARRAY_H

#include <stddef.h>

#include "JSON/JSON.h"

struct TokenArray *create_token_array();
size_t get_token_array_size(struct TokenArray *arr);
void expand_token_array(struct TokenArray *arr, struct Token *token);
struct Token *get_token_array_element(struct TokenArray *arr, size_t n);
void destroy_token_array(struct TokenArray *arr);

#endif /* TOKEN_ARRAY_H */
