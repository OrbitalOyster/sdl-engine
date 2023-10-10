#ifndef WTREE_H
#define WTREE_H

#define MAX_WORD_LENGTH 64
#define MAX_TREE_SIZE 200000

struct WTreeNode {
  char c;
  struct WTreeNode *parent;
  unsigned int size;
  struct WTreeNode **children;
  void *endpoint;
};

struct WTree {
  struct WTreeNode *root;
  unsigned int size;
  char **words;
};

struct WTree *createWTree();
struct WTreeNode *createNode(struct WTreeNode *parent, char c);
int expandWTree(struct WTree *tree, char *word, void *endpoint);
void *getWTreeEndpoint(struct WTree *tree, char *word);
void destroyTree(struct WTree *tree);

#endif
