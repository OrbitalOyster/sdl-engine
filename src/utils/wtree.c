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
  INFOF("Seaching node %s, size %u for %c", node->s, node->size, c);
  for (unsigned short int i = 0; i < node->size; i++)
    if (node->children[i]->s[0] == c)
      return node->children[i];
  return NULL;
}

// Returns number of matching chars
unsigned int compareWords(char *s1, char *s2) {
  INFOF("Comparing %s to %s", s1, s2);
  unsigned int n = 0;
  int nullChar = 0;
  while (!nullChar)
    if (s1[n] != s2[n]) {
      INFOF("%c (%i) != %c (%i)", s1[n], s1[n], s2[n], s2[n]);
      INFOF("Matched %u chars", n);
      return n;
    } else {
      nullChar = (s1[n] == '\0' || s2[n] == '\0');
      n++;
    }
  INFOF("Matched %u chars", n);
  return n;
}

void appendNode(struct WTreeNode *parent, char *word) {
  INFO2F("Appending node %s to %s", word, parent->s);
  struct WTreeNode *child = createNode(parent, word);
  parent->size++;
  parent->children =
      realloc(parent->children, parent->size * sizeof(struct WTreeNode *));
  parent->children[parent->size - 1] = child;
}

void splitNode(struct WTreeNode *node, unsigned int n) {
  INFOF("Splitting node %s by char #%i", node->s, n);
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
  node->children = calloc(1, sizeof(struct WTreeNode *));
  node->children[0] = tail;
}

// Removes first n char of string, size l (including null character)
char *trimString(char *s, unsigned int n, unsigned int l) {
  INFOF("Trimming %s, size %u, by %u chars", s, l, n);
  for (unsigned int i = 0; i < n + 1; i++)
    s[i] = s[i + n];
  s[l - n - 1] = '\0';
  return realloc(s, (l - n) * sizeof(char));
}

int expandWTree(struct WTree *tree, char *word, void *endpoint) {
  INFOF("Expanding by %s", word);
  char *tail = calloc(strlen(word) + 1, sizeof(char));
  strcpy(tail, word);
  struct WTreeNode *parentNode = tree->root;
  struct WTreeNode *node = getChild(parentNode, word[0]);

  while (1) {
    // Entirely new tail, append to node
    if (!node) {
      INFO("Child not found, appending tail");
      node = parentNode;
      break;
    }
    INFOF("Switched to node: %s", node->s);
    unsigned int tl = (unsigned int)strlen(tail) + 1;
    unsigned int matched = compareWords(node->s, tail);
    // abcdef/abc
    if (tail[matched] == '\0') {
      // abc/abc
      if (node->s[matched] == '\0') {
        INFO("New node is the same like existing one");
        if (!node->size)
          ERR(1, "Duplicate");
      } else {
        INFO("New node is shorter than existing one");
        splitNode(node, matched);
      }
      tail = realloc(tail, sizeof(char));
      tail[0] = '\0';
      break;
    }
    // Trim tail
    tail = trimString(tail, matched, tl);
    INFOF("New tail: %s", tail);
    // abcdef/abcxxx
    if (node->s[matched] != '\0') {
      INFOF("New branch, %s", tail);
      splitNode(node, matched);
      break;
    }
    // abc/abcdef
    if (!node->size) {
      INFO("New node is longer than existing one");
      char *nullChar = calloc(1, sizeof(char));
      nullChar[0] = '\0';
      appendNode(node, nullChar);
      break;
    }
    parentNode = node;
    node = getChild(node, tail[0]);
    INFO("Next cycle...");
  }

  // Done
  appendNode(node, tail);
  tree->size++;
  node->endpoint = endpoint;
  INFO2("Expand complete");
  return 0;
}

// Recursive function, fills *word pointer with chars following tree until '\0'
/*
void getNodeWord(struct WTreeNode *node, char *word, unsigned int level) {
  word[level] = node->c;
  word = realloc(word, (level + 2) * sizeof(char));
  // End of branch, add '\0'
  if (node->size == 0)
    word[level + 1] = '\0';
  else {
    level++;
    for (unsigned short int i = 0; i < node->size; i++)
      getNodeWord(node->children[i], word, level);
  }
}

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
  if (!node)
    return NULL;
  while (1) {
    unsigned int matched = compareWords(node->s, tail);
    if (!node->size && node->s[matched] == '\0' && tail[matched] == '\0')
      return node->endpoint;
    trimString(tail, matched, (unsigned int)strlen(tail) + 1);
  }
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
