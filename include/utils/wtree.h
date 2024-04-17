#ifndef WTREE_H
#define WTREE_H

#include <stddef.h>

#define WTREE_CHARS_NUMBER 64
#define WTREE_CHARS                                                            \
  "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"

struct WTree *create_wtree();
void expand_wtree(struct WTree *wtree, char *word, void *endpoint);
void shrink_wtree(struct WTree *wtree, char *word);
size_t get_wtree_size(struct WTree *wtree);
void *get_wtree_endpoint(struct WTree *wtree, char *word);
char **get_wtree_words(struct WTree *wtree);
void destroy_wtree(struct WTree *wtree);

#endif /* WTREE_H */
