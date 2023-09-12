#ifndef WTREE_H
#define WTREE_H

struct WTreeNode {
  char c;
  struct WTreeNode *parent;
  unsigned int size;
  struct WTreeNode **children;
};

struct WTree {
  struct WTreeNode *root;
  unsigned int size;
  void **endpoints;
};

struct WTree *createWTree();
struct WTreeNode *createNode(struct WTreeNode *parent, char c);
int expandWTree(struct WTree *node, char *word);

#endif
