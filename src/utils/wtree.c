#include "utils/wtree.h"

#include <stdlib.h>

#include "utils/debug.h"

int checkIfValidChar(char c) {
  return (c > '0' && c < '9') || (c > 'a' && c < 'z') || (c > 'A' && c < 'Z') ||
         c == '_';
}

struct WTree *createWTree() {
  struct WTree *result = calloc(1, sizeof(struct WTree));
  result->root = calloc(1, sizeof(struct WTreeNode));
  *(result->root) =
      (struct WTreeNode){.parent = NULL, .c = -1, .size = 0, .children = NULL};
  result->size = 0;
  result->endpoints = NULL;
  return result;
}

struct WTreeNode *createNode(struct WTreeNode *parent, char c) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  *result =
      (struct WTreeNode){.parent = parent, .c = c, .size = 0, .children = NULL};
  return result;
}

// TODO Less brutal approach
struct WTreeNode *getChild(struct WTreeNode *node, char c) {
  // TODO: Debug flag
  if (node == NULL) {
    WARN("Attempt to access NULL node");
    return NULL;
  }
  for (unsigned int i = 0; i < node->size; i++)
    if (node->children[i]->c == c)
      return node->children[i];
  return NULL;
}

int expandWTree(struct WTree *tree, char *word) {
  int done = 0;
  int n = 0;

  struct WTreeNode *currentNode = tree->root;

  while (!done) {
    char c = word[n];
    INFOF("c: %c", c);
    if (c == '\0')
      done = 1;
    else {
      struct WTreeNode *nextNode = getChild(currentNode, c);
      if (nextNode == NULL) {
        nextNode = createNode(currentNode, c);
        currentNode->size++;
        currentNode->children =
            realloc(currentNode->children,
                    currentNode->size * sizeof(struct WTreeNode *));
        if (currentNode->children == NULL) {
          WARN("Unable to allocate memory");
          return 1;
        }
        currentNode->children[currentNode->size - 1] = nextNode;
      }
      currentNode = nextNode;
      n++;
    }
  }
  return 0;
}
