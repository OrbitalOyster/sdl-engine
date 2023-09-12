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
  return result;
}

struct WTreeNode *createNode(struct WTreeNode *parent, char c) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  *result = (struct WTreeNode){
      .parent = parent, .c = c, .size = 0, .children = NULL, .endpoint = NULL};
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

int expandWTree(struct WTree *tree, char *word, void *endpoint) {
  int n = 0;
  struct WTreeNode *currentNode = tree->root;
  while (1) {
    char c = word[n++];
    if (c == '\0') {
      currentNode->endpoint = endpoint;
      return 0;
    }
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
  }
}

void *getEndpoint(struct WTree *tree, char *word) {
  int n = 0;
  struct WTreeNode *currentNode = tree->root;
  while (1) {
    char c = word[n++];
    if (c == '\0')
      return currentNode->endpoint;
    struct WTreeNode *nextNode = getChild(currentNode, c);
    if (nextNode == NULL)
      return NULL;
    currentNode = nextNode;
  }
}

void destroyNode(struct WTreeNode *node) {
  for (unsigned int i = 0; i < node->size; i++)
    destroyNode(node->children[i]);
  free(node);
}

void destroyTree(struct WTree *tree) {
  destroyNode(tree->root);
  free(tree);
}
