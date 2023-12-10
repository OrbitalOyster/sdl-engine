#ifndef WTREE_H
#define WTREE_H

#define WTREE_CHARS_NUMBER 64
#define WTREE_CHARS                                                            \
  "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"
#define WTREE_MAX_WORD_LENGTH 64

struct WTree;

struct WTree *createWTree();
void expandWTree(struct WTree *tree, char *word, void *endpoint);
void sortWTree(struct WTree *tree);
void *getWTreeEndpoint(struct WTree *tree, char *word);
void debugWTree(struct WTree *tree);
char **getWTreeWords(struct WTree *tree);
void destroyWTree(struct WTree *tree);

#endif /* WTREE_H */
