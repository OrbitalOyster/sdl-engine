#ifndef WTREE_H
#define WTREE_H

struct WTreeNode {
  char c;
  struct WTreeNode *parent;
  unsigned int size;
  struct WTreeNode **children;
  void *endpoint;
};

struct WTree {
  struct WTreeNode *root;
};

struct WTree *createWTree();
struct WTreeNode *createNode(struct WTreeNode *parent, char c);
int expandWTree(struct WTree *node, char *word, void *endpoint);
void *getEndpoint(struct WTree *tree, char *word);
void destroyTree(struct WTree *tree);

#endif
