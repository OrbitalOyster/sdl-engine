#include "utils/wtree.h"

#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/qsort.h"

struct WTree *createWTree() {
  struct WTree *result = calloc(1, sizeof(struct WTree));
  result->root = calloc(1, sizeof(struct WTreeNode));
  *(result->root) =
      (struct WTreeNode){.parent = NULL, .c = -1, .size = 0, .children = NULL};
  result->size = 0;
  result->words = NULL;
  return result;
}

struct WTreeNode *createNode(struct WTreeNode *parent, char c) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTreeNode){
      .parent = parent, .c = c, .size = 0, .children = NULL, .endpoint = NULL};
  return result;
}


int nodeSortFunc(void **arr, int i1, int i2) {
  return ((struct WTreeNode *)arr[i1])->c < ((struct WTreeNode *)arr[i2])->c;
}

void sortNode(struct WTreeNode *node) {
  sort((void **)node->children, 0, (int)node->size-1, nodeSortFunc);
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
  char *nextWord = calloc(strlen(word), sizeof(char));
  if (nextWord == NULL) {
    WARN("Unable to allocate memory");
    return 1;
  }
  unsigned int n = 0;
  struct WTreeNode *currentNode = tree->root;
  while (1) {
    char c = word[n];
    nextWord[n] = c;
    n++;
    if (c == '\0') {
      currentNode->endpoint = endpoint;
      tree->size++;
      tree->words = realloc(tree->words, tree->size * sizeof(char *));
      tree->words[tree->size - 1] = nextWord;
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

void *getWTreeEndpoint(struct WTree *tree, char *word) {
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
  free(node->children);
  free(node);
}

void destroyWTree(struct WTree *tree) {
  destroyNode(tree->root);
  for (unsigned int i = 0; i < tree->size; i++)
    free(tree->words[i]);
  free(tree->words);
  free(tree);
}
