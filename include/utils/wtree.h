#ifndef WTREE_H
#define WTREE_H

#define WTREE_CHARS_NUMBER 64
#define WTREE_CHARS                                                            \
  "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"
#define WTREE_MAX_WORD_LENGTH 64

struct WTree *create_wtree();
void expand_wtree(struct WTree *wtree, char *word, void *endpoint);
void shrink_wtree(struct WTree *wtree, char *word);
unsigned int get_wtree_size(struct WTree *wtree);
void sort_wtree(struct WTree *wtree);
void *get_wtree_endpoint(struct WTree *wtree, char *word);
char **get_wtree_words(struct WTree *wtree);
void destroy_wtree(struct WTree *wtree);

#endif /* WTREE_H */
