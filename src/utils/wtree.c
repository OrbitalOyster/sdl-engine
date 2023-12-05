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
  // return ((struct WTreeNode *)arr[i1])->c < ((struct WTreeNode *)arr[i2])->c;
}

void sortNode(struct WTreeNode *node) {
  sort((void **)node->children, 0, (int)node->size - 1, nodeSortFunc);
  for (unsigned short int i = 0; i < node->size; i++)
    sortNode(node->children[i]);
}

// TODO Less brutal approach
struct WTreeNode *getChild(struct WTreeNode *node, char *s) {
  // TODO: Debug flag
  if (node == NULL) {
    WARN("Attempt to access NULL node");
    return NULL;
  }
  INFOF("Seaching node %s, size %u for %s", node->s, node->size, s);
  for (unsigned short int i = 0; i < node->size; i++)
    if (node->children[i]->s[0] == s[0])
      return node->children[i];
  return NULL;
}

/*
int expandWTree(struct WTree *tree, char *word, void *endpoint) {
  char *nextWord = calloc(strlen(word) + 1, sizeof(char));
  if (nextWord == NULL) {
    WARN("Unable to allocate memory");
    return 1;
  }
  unsigned int n = 0;
  unsigned int level = 0;
  struct WTreeNode *currentNode = tree->root;
  while (1) {
    char c = word[n];
    nextWord[n] = c;
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
    n++;
    currentNode = nextNode;
  }
}
*/

// Returns number of matched chars
unsigned int compareWords(char *s1, char *s2) {
  INFOF("Comparing %s to %s", s1, s2);
  unsigned int n = 0;
  int gotNull = 0;
  while (!gotNull)
    if (s1[n] != s2[n]) {
      INFOF("%c (%i) != %c (%i)", s1[n], s1[n], s2[n], s2[n]);
      return n;
    } else {
      gotNull = (s1[n] == '\0' || s2[n] == '\0');
      n++;
    }
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

void appendNullNode(struct WTreeNode *parent) {
  INFO2F("Appending null node to %s", parent->s);
  char *nullWord = calloc(1, sizeof(char));
  nullWord[0] = '\0';
  struct WTreeNode *child = createNode(parent, nullWord);
  parent->size++;
  parent->children =
      realloc(parent->children, parent->size * sizeof(struct WTreeNode *));
  parent->children[parent->size - 1] = child;
}

struct WTreeNode *splitNode(struct WTreeNode *node, unsigned int n) {
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
  struct WTreeNode *tail = createNode(node, stail);
  // Creating tail
  tail->size = node->size;
  tail->children = calloc(tail->size, sizeof(struct WTreeNode *));
  for (unsigned int i = 0; i < tail->size; i++)
    tail->children[i] = node->children[i];
  // Replacing head
  node->size = 1;
  free(node->s);
  node->s = shead;
  free(node->children);
  node->children = calloc(1, sizeof(struct WTreeNode *));
  node->children[0] = tail;
  return tail;
}

int expandWTree(struct WTree *tree, char *word, void *endpoint) {
  if (endpoint) {
  };
  INFOF("Expanding by %s", word);
  char *tail = calloc(strlen(word) + 1, sizeof(char));
  strcpy(tail, word);

  struct WTreeNode *parentNode = tree->root;
  struct WTreeNode *node = getChild(parentNode, word);

  while (1) {
    // Entirely new word, append to node
    if (!node) {
      INFO("Child not found, appending tail");
      appendNode(parentNode, tail);
      tree->size++;
      INFO2("Expand complete");
      return 0;
    }

    INFOF("Switched to node: %s", node->s);
    unsigned int tl = (unsigned int)strlen(tail) + 1;
    unsigned int matched = compareWords(node->s, tail);
    INFOF("Matched %u chars", matched);
    // Error
    if (tail[matched] == '\0' && node->s[matched] == '\0') {
      if (!node->size) ERR(1, "Duplicate");
      appendNullNode(node);
			tree->size++;
      INFO2("Expand complete");
      return 0;
		}
    // abcdef/abc
    if (tail[matched] == '\0') {
      INFO("New node is shorter than existing one");
      splitNode(node, matched);
      appendNullNode(node);
      tree->size++;
      INFO2("Expand complete");
      return 0;
    }
    // abc/abcdef
    if (node->s[matched] == '\0' && tail[matched] != '\0' && !node->size) {
      INFO("New node is longer than existing one");
			appendNullNode(node);
      // Trim tail
      for (unsigned int i = 0; i < matched + 1; i++)
        tail[i] = tail[i + matched];
      tail[tl - matched - 1] = '\0';
      tail = realloc(tail, (tl - matched) * sizeof(char));
      INFOF("New tail: %s", tail);
      appendNode(node, tail);
			tree->size++;
      INFO2("Expand complete");
      return 0;
    }
    // abcdef/abcxxx
    if (node->s[matched] != '\0' && tail[matched] != '\0') {
	    // Trim tail
			for (unsigned int i = 0; i < matched + 1; i++)
				tail[i] = tail[i + matched];
			tail[tl - matched - 1] = '\0';
			tail = realloc(tail, (tl - matched) * sizeof(char));
			INFOF("New tail: %s", tail);

			INFOF("New branch, %s", tail);
      splitNode(node, matched);
			appendNode(node, tail);
			tree->size++;
      INFO2("Expand complete");
      return 0;
		}
    // Trim tail
    for (unsigned int i = 0; i < matched + 1; i++)
      tail[i] = tail[i + matched];
    tail[tl - matched - 1] = '\0';
    tail = realloc(tail, (tl - matched) * sizeof(char));
    INFOF("New tail: %s", tail);

    parentNode = node;
    node = getChild(node, tail);
    INFO("Next cycle...");
  }
}

/*
int expandWTree(struct WTree *tree, char *word, void *endpoint) {
  if (endpoint) {
  };
  INFOF("Expanding by %s", word);
  char *tail = calloc(strlen(word) + 1, sizeof(char));
  unsigned int n = 0;
  strcpy(tail, word);
  struct WTreeNode *currentNode = tree->root;
  struct WTreeNode *nextNode = getChild(currentNode, tail);
  // New node entirely
  if (nextNode == NULL) {
    INFOF("Node not found, adding %s", tail);
    appendNode(currentNode, tail);
    tree->size++;
    return 0; // Done
  }
  INFOF("Next node: %s", nextNode->s);
  unsigned int tl = (unsigned int)strlen(tail) + 1;
  unsigned int l = compareWords(nextNode->s, tail, n, tl);
  INFOF("l == %u", l);
  // Should not happen
  if (tl < l)
    ERR(1, "Should not happen");

  if (tail[l] == '\0' && nextNode->s[l] == '\0')
    ERR(1, "Duplicate");

  // abcdef, abc
  if (tail[l] == '\0') {
    splitNode(nextNode, l);
    appendNode(nextNode, '\0'); // TODO just add '\0' to nextNode->s ?
    return 0;
  }

  char *newTail = calloc(tl - l, sizeof(char));
  for (unsigned i = 0; i < tl - l; i++)
    newTail[i] = tail[i + l];
  INFOF("New tail: %s", newTail);
  // abc, abcdef
  if (nextNode->s[l] == '\0') {
    ERR(1, "Go to x");
    appendNode(nextNode, newTail);
    tree->size++;
    return 0;
  }

  // abcdef, abcccc
  splitNode(nextNode, l);
  appendNode(nextNode, newTail);
  tree->size++;
  return 0;
}
*/

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
  if (tree && word) {
  };
  return NULL;
}

/*
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
*/

void destroyNode(struct WTreeNode *node) {
  for (unsigned short int i = 0; i < node->size; i++)
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
