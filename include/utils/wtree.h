#ifndef WTREE_H
#define WTREE_H

#define WTREE_CHARS_NUMBER 64
#define WTREE_CHARS                                                            \
  "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"

struct WTreeNode {
  char *s;
  struct WTreeNode *parent;
  unsigned short int size;
  struct WTreeNode **children;
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
void resetWTreeWords(struct WTree *tree);
void *getWTreeEndpoint(struct WTree *tree, char *word);
void destroyWTree(struct WTree *tree);

#endif /* WTREE_H */
