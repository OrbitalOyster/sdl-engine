#ifndef WTREE_H
#define WTREE_H

#define WTREE_CHARS_NUMBER 64
#define WTREE_CHARS                                                            \
  "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"

struct WTreeNode {
  char c;
  struct WTreeNode *parent;
  unsigned int size;
  struct WTreeNode **children;
  unsigned int memoryAllocated;
  void *endpoint;
};

struct WTree {
  struct WTreeNode *root;
  unsigned int size;
  char **words;
};

struct WTree *createWTree();
int expandWTree(struct WTree *tree, char *word, void *endpoint);
void getWTreeWords(struct WTree *tree);
void sortWTree(struct WTree *tree);
void *getWTreeEndpoint(struct WTree *tree, char *word);
void destroyWTree(struct WTree *tree);

#endif
