#include "utils/wtree.h"

#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/qsort.h"

#include <stdio.h>

void destroyNode(struct WTreeNode *node);

struct WTreeNode *createNode(struct WTreeNode *parent, char *s) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTreeNode){
      .parent = parent, .s = s, .size = 0, .children = NULL, .endpoint = NULL};
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
  sort((void **)node->children, 0, (int)node->size - 1, nodeSortFunc);
  for (unsigned short int i = 0; i < node->size; i++)
    sortNode(node->children[i]);
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
    if (node->children[i]->s[0] == c)
      return node->children[i];
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
  parent->children =
      realloc(parent->children, parent->size * sizeof(struct WTreeNode *));
  parent->children[parent->size - 1] = child;
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
  tail->endpoint = node->endpoint;
  // Replacing head
  node->size = 1;
  free(node->s);
  node->endpoint = NULL;
  node->s = shead;
  node->children = calloc(1, sizeof(struct WTreeNode *));
  node->children[0] = tail;
}

// Removes first n char of string
char *trimString(char *s, unsigned int n) {
  INFOF("Trimming %s by %u chars", s, n);
  unsigned int i = 0;
  for (; i < n + 1; i++)
    s[i] = s[i + n];
  s[i] = '\0';
  return realloc(s, (i + 1) * sizeof(char));
}

int expandWTree(struct WTree *tree, char *word, void *endpoint) {
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
  appended->endpoint = endpoint;
  tree->size++;
  INFO2("Expand complete");
  return 0;
}

/*
void resetWTreeWords(struct WTree *tree) {
  // At least 1 char ('\0')
  unsigned int level = 0;
  for (unsigned short int i = 0; i < tree->root->size; i++) {
    free(tree->words[i]);
    tree->words[i] = calloc(1, sizeof(char));
    tree->words[i][0] = '\0';
    getNodeWord(tree->root->children[i], tree->words[i], level);
  }
}
*/

void sortWTree(struct WTree *tree) { sortNode(tree->root); }

void *getWTreeEndpoint(struct WTree *tree, char *word) {
  char *tail = calloc(strlen(word) + 1, sizeof(char));
  strcpy(tail, word);
  struct WTreeNode *node = getChild(tree->root, tail[0]);
  while (node) {
    INFOF("Searching node %s", node->s);
    unsigned int matched = compareWords(node->s, tail);
    if (!node->size && node->s[matched] == '\0' && tail[matched] == '\0') {
      INFO2("Found it");
      return node->endpoint;
    }
    tail = trimString(tail, matched);
    node = getChild(node, tail[0]);
  }
  return NULL;
}

void debugWord(struct WTreeNode *node, char *word, unsigned int *n) {
  unsigned int l = 0;
  if (node->s) {
    l = (unsigned int)strlen(node->s);
    strcat(word, node->s);
  }
  *n += l;
  if (!node->size)
    INFO2F("%s", word)
  else
    for (unsigned int i = 0; i < node->size; i++)
      debugWord(node->children[i], word, n);
  *n -= l;
  word[*n] = '\0';
}

void debugWTree(struct WTree *tree) {
  char *word = calloc(64, sizeof(char));
  unsigned int n = 0;
  memset(word, '\0', 64);
  struct WTreeNode *node = tree->root;
  debugWord(node, word, &n);
}

void destroyNode(struct WTreeNode *node) {
  free(node->s);
  for (unsigned short int i = 0; i < node->size; i++)
    destroyNode(node->children[i]);
  free(node->children);
  free(node);
}

void destroyWTree(struct WTree *tree) {
  destroyNode(tree->root);
  //  for (unsigned int i = 0; i < tree->size; i++)
  //    free(tree->words[i]);
  free(tree->words);
  free(tree);
}
