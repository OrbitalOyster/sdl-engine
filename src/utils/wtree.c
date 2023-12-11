#include "utils/wtree.h"

#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/qsort.h"

#include <stdio.h>

union WTreeChildren {
  struct WTreeNode **nodes;
  void *endpoint;
};

struct WTreeNode {
  char *s;
  struct WTreeNode *parent;
  unsigned short int size;
  union WTreeChildren children;
};

struct WTree {
  struct WTreeNode *root;
  unsigned int size;
  char **words;
};

void destroyNode(struct WTreeNode *node);

struct WTreeNode *createNode(struct WTreeNode *parent, char *s) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTreeNode){
      .parent = parent, .s = s, .size = 0, .children.nodes = NULL};
  return result;
}

struct WTree *createWTree() {
  struct WTree *result = calloc(1, sizeof(struct WTree));
  result->root = createNode(NULL, NULL);
  result->size = 0;
  result->words = NULL;
  return result;
}

int nodeSortFunc(void **arr, int i1, int i2) {
  return strcmp(((struct WTreeNode *)arr[i1])->s,
                ((struct WTreeNode *)arr[i2])->s) < 0;
}

void sortNode(struct WTreeNode *node) {
  sort((void **)node->children.nodes, 0, (int)node->size - 1, nodeSortFunc);
  for (unsigned short int i = 0; i < node->size; i++)
    sortNode(node->children.nodes[i]);
}

// TODO Less brutal approach
struct WTreeNode *getChild(struct WTreeNode *node, char c) {
  // TODO: Debug flag
  if (node == NULL) {
    WARN("Attempt to access NULL node");
    return NULL;
  }
  INFOF("Seaching node %s, size %u for %c(%i)", node->s, node->size, c, c);
  for (unsigned short int i = 0; i < node->size; i++)
    if (node->children.nodes[i]->s[0] == c)
      return node->children.nodes[i];
  return NULL;
}

// Returns number of matching chars (excluding null chars)
unsigned int compareWords(char *s1, char *s2) {
  INFOF("Comparing %s to %s", s1, s2);
  unsigned int n = 0;
  do {
    if (s1[n] == '\0' || s2[n] == '\0' || s1[n] != s2[n]) {
      INFOF("%c (%i) != %c (%i)", s1[n], s1[n], s2[n], s2[n]);
      INFOF("Matched %u chars", n);
      return n;
    }
  } while (++n);
  return n; // Should not happen
}

struct WTreeNode *appendNode(struct WTreeNode *parent, char *word) {
  INFO2F("Appending node %s to %s", word, parent->s);
  struct WTreeNode *child = createNode(parent, word);
  parent->size++;
  parent->children.nodes = realloc(parent->children.nodes,
                                   parent->size * sizeof(struct WTreeNode *));
  parent->children.nodes[parent->size - 1] = child;
  return child;
}

void splitNode(struct WTreeNode *node, unsigned int n) {
  INFO2F("Splitting node %s by char #%i", node->s, n);
  // Splitting word
  unsigned int l = (unsigned int)strlen(node->s);
  l++; // null character
  char *shead = calloc(n + 1, sizeof(char));
  char *stail = calloc(l - n, sizeof(char));
  for (unsigned int i = 0; i < n; i++)
    shead[i] = node->s[i];
  shead[n] = '\0';
  for (unsigned int i = n; i < l; i++)
    stail[i - n] = node->s[i];
  INFOF("shead == %s, stail == %s", shead, stail);
  // Creating tail
  struct WTreeNode *tail = createNode(node, stail);
  tail->size = node->size;
  tail->children = node->children;
  // Replacing head
  node->size = 1;
  free(node->s);
  node->s = shead;
  node->children.nodes = calloc(1, sizeof(struct WTreeNode *));
  node->children.nodes[0] = tail;
}

// Removes first n char of string
char *trimString(char *s, unsigned int n) {
  INFOF("Trimming %s by %u chars", s, n);
  unsigned int l = (unsigned int)strlen(s);
  unsigned int i = 0;
  for (; i < l - n; i++)
    s[i] = s[i + n];
  s[i] = '\0';
  return realloc(s, (i + 1) * sizeof(char));
}

void expandWTree(struct WTree *tree, char *word, void *endpoint) {
  INFOF("Expanding by %s", word);
  // Tail length, + 1 for null char
  unsigned int tl = (unsigned int)strlen(word) + 1;
  char *tail = calloc(tl, sizeof(char));
  strcpy(tail, word);
  struct WTreeNode *node = tree->root;
  struct WTreeNode *nextNode = getChild(node, tail[0]);
  while (nextNode) {
    INFOF("Switched to node: %s", nextNode->s);
    unsigned int l = (unsigned int)strlen(nextNode->s);
    unsigned int matched = compareWords(nextNode->s, tail);
    INFOF("tl == %u, l == %u, matched == %u", tl, l, matched);
    tail = trimString(tail, matched);
    tl -= matched;
    if (matched < l || (matched == l && !nextNode->size))
      splitNode(nextNode, matched);
    node = nextNode;
    nextNode = getChild(node, tail[0]);
  }
  INFOF("Child not found, appending tail %s", tail);
  struct WTreeNode *appended = appendNode(node, tail);
  appended->children.endpoint = endpoint;
  tree->size++;
  INFO2("Expand complete");
}

unsigned int getWTreeSize(struct WTree* tree) {
  return tree->size;
}

void sortWTree(struct WTree *tree) { sortNode(tree->root); }

struct WTreeNode *searchWTree(struct WTree *tree, char *word) {
  char *tail = calloc(strlen(word) + 1, sizeof(char));
  strcpy(tail, word);
  struct WTreeNode *node = getChild(tree->root, tail[0]);
  while (node) {
    unsigned int matched = compareWords(node->s, tail);
    if (!node->size && node->s[matched] == '\0' && tail[matched] == '\0') {
      free(tail);
      return node;
    }
    tail = trimString(tail, matched);
    node = getChild(node, tail[0]);
  }
  free(tail);
  return NULL;
}

void *getWTreeEndpoint(struct WTree *tree, char *word) {
  struct WTreeNode *node = searchWTree(tree, word);
  if (!node)
    return NULL;
  else
    return node->children.endpoint;
}

void getWTreeWord(struct WTreeNode *node, char *word, unsigned int *n,
                  unsigned int *size, char **result) {
  unsigned int l = 0;
  if (node->s) {
    l = (unsigned int)strlen(node->s);
    strcat(word, node->s);
  }
  *n += l;
  if (!node->size) {
    char *newWord = calloc(*n + 1, sizeof(char));
    strcpy(newWord, word);
    result[(*size)++] = newWord;
  } else
    for (unsigned int i = 0; i < node->size; i++)
      getWTreeWord(node->children.nodes[i], word, n, size, result);
  *n -= l;
  word[*n] = '\0';
}

char **getWTreeWords(struct WTree *tree) {
  char **result = calloc(tree->size, sizeof(char *));
  unsigned int size = 0;
  char *word = calloc(WTREE_CHARS_NUMBER, sizeof(char));
  unsigned int n = 0;
  memset(word, '\0', WTREE_CHARS_NUMBER);
  struct WTreeNode *node = tree->root;
  getWTreeWord(node, word, &n, &size, result);
  free(word);
  return result;
}

void destroyNode(struct WTreeNode *node) {
  free(node->s);
  if (node->size) {
    for (unsigned short int i = 0; i < node->size; i++)
      destroyNode(node->children.nodes[i]);
    free(node->children.nodes);
  }
  free(node);
}

void destroyWTree(struct WTree *tree) {
  destroyNode(tree->root);
  //  for (unsigned int i = 0; i < tree->size; i++)
  //    free(tree->words[i]);
  free(tree->words);
  free(tree);
}
