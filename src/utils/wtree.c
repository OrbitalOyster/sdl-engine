#include "utils/wtree.h"

#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/dstrings.h"
#include "utils/qsort.h"

union WTreeChildren {
  struct WTreeNode **nodes;
  void *endpoint;
};

struct WTreeNode {
  char *chunk;
  struct WTreeNode *parent;
  unsigned short int size;
  union WTreeChildren children;
};

struct WTree {
  struct WTreeNode *root;
  unsigned int size;
};

static void destroy_node(struct WTreeNode *node);

static struct WTreeNode *create_node(struct WTreeNode *parent, char *chunk) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTreeNode){
      .parent = parent, .chunk = chunk, .size = 0, .children.nodes = NULL};
  return result;
}

struct WTree *create_wtree() {
  struct WTree *result = calloc(1, sizeof(struct WTree));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTree){.root = create_node(NULL, NULL), .size = 0};
  return result;
}

// Which node should come first?
static int node_sort_func(void **arr, int i1, int i2) {
  return strcmp(((struct WTreeNode *)arr[i1])->chunk,
                ((struct WTreeNode *)arr[i2])->chunk) < 0;
}

static void sort_node(struct WTreeNode *node) {
  sort((void **)node->children.nodes, 0, (int)node->size - 1, node_sort_func);
  for (unsigned short int i = 0; i < node->size; i++)
    sort_node(node->children.nodes[i]);
}

// TODO Less brutal approach
static struct WTreeNode *get_child(struct WTreeNode *node, char c) {
  // TODO: Debug flag
  if (node == NULL) {
    WARN("Attempt to access NULL node");
    return NULL;
  }
  INFOF("Seaching node %s, size %u for %c(%i)", node->chunk, node->size, c, c);
  for (unsigned short int i = 0; i < node->size; i++)
    if (node->children.nodes[i]->chunk[0] == c)
      return node->children.nodes[i];
  return NULL;
}

// Creates new node (chuck) and appends it to parent
static struct WTreeNode *append_node(struct WTreeNode *parent, char *chunk) {
  INFO2F("Appending node %s to %s", chunk, parent->chunk);
  struct WTreeNode *child = create_node(parent, chunk);
  parent->size++;
  parent->children.nodes = realloc(parent->children.nodes,
                                   parent->size * sizeof(struct WTreeNode *));
  if (parent->children.nodes = NULL)
    ERR(1, "Out of memory");
  parent->children.nodes[parent->size - 1] = child;
  return child;
}

// Creates new node by splitting existing node by n chars (first n goes to the
// head)
static void split_node(struct WTreeNode *node, unsigned int n) {
  INFO2F("Splitting node %s by char #%i", node->chunk, n);
  // Splitting word
  unsigned int l = (unsigned int)strlen(node->chunk) + 1; // + null char
  char *head_chunk = calloc(n + 1, sizeof(char));
  char *tail_chunk = calloc(l - n, sizeof(char));
  if (head_chunk == NULL || tail_chunk == NULL)
    ERR(1, "Out of memory");
  for (unsigned int i = 0; i < n; i++)
    head_chunk[i] = node->chunk[i];
  head_chunk[n] = '\0';
  for (unsigned int i = n; i < l; i++)
    tail_chunk[i - n] = node->chunk[i];
  INFOF("head == %s, tail == %s", head_chunk, tail_chunk);
  // Creating tail
  struct WTreeNode *tail = create_node(node, tail_chunk);
  tail->size = node->size;
  tail->children = node->children;
  // Replacing head
  node->size = 1;
  free(node->chunk);
  node->chunk = head_chunk;
  node->children.nodes = calloc(1, sizeof(struct WTreeNode *));
  if (node->children.nodes == NULL)
    ERR(1, "Out of memory");
  node->children.nodes[0] = tail;
}

void expand_wtree(struct WTree *wtree, char *word, void *endpoint) {
  INFOF("Expanding by %s", word);
  // Tail length, + 1 for null char
  unsigned int tl = (unsigned int)strlen(word) + 1;
  char *tail = calloc(tl, sizeof(char));
  if (tail == NULL)
    ERR(1, "Out of memory");
  strcpy(tail, word);
  struct WTreeNode *node = wtree->root;
  struct WTreeNode *next_node = get_child(node, tail[0]);
  while (next_node) {
    INFOF("Switched to node: %s", next_node->chunk);
    unsigned int l = (unsigned int)strlen(next_node->chunk);
    unsigned int matched = get_str_match(next_node->chunk, tail);
    INFOF("tl == %u, l == %u, matched == %u", tl, l, matched);
    tail = trim_str(tail, matched);
    tl -= matched;
    if (matched < l || (matched == l && !next_node->size))
      split_node(next_node, matched);
    node = next_node;
    next_node = get_child(node, tail[0]);
  }
  INFOF("Child not found, appending tail %s", tail);
  struct WTreeNode *appended = append_node(node, tail);
  appended->children.endpoint = endpoint;
  wtree->size++;
  INFO2("Expand complete");
}

unsigned int get_wtree_size(struct WTree *wtree) { return wtree->size; }

void sort_wtree(struct WTree *wtree) { sort_node(wtree->root); }

struct WTreeNode *search_wtree(struct WTree *wtree, char *word) {
  char *tail = calloc(strlen(word) + 1, sizeof(char));
  if (tail == NULL)
    ERR(1, "Out of memory");
  strcpy(tail, word);
  struct WTreeNode *node = get_child(wtree->root, tail[0]);
  while (node) {
    unsigned int matched = get_str_match(node->chunk, tail);
    if (!node->size && node->chunk[matched] == '\0' && tail[matched] == '\0') {
      free(tail);
      return node;
    }
    tail = trim_str(tail, matched);
    node = get_child(node, tail[0]);
  }
  free(tail);
  return NULL;
}

void *get_wtree_endpoint(struct WTree *wtree, char *word) {
  struct WTreeNode *node = search_wtree(wtree, word);
  if (!node)
    return NULL;
  else
    return node->children.endpoint;
}

static void get_wtree_word(struct WTreeNode *node, char *word, unsigned int *n,
                           unsigned int *size, char **result) {
  unsigned int l = 0;
  if (node->chunk) {
    l = (unsigned int)strlen(node->chunk);
    strcat(word, node->chunk);
  }
  *n += l;
  if (!node->size) {
    char *new_word = calloc(*n + 1, sizeof(char));
    if (new_word == NULL)
      ERR(1, "Out of memory");
    strcpy(new_word, word);
    result[(*size)++] = new_word;
  } else
    for (unsigned int i = 0; i < node->size; i++)
      get_wtree_word(node->children.nodes[i], word, n, size, result);
  *n -= l;
  word[*n] = '\0';
}

char **get_wtree_words(struct WTree *wtree) {
  char **result = calloc(wtree->size, sizeof(char *));
  if (result == NULL)
    ERR(1, "Out of memory");
  unsigned int size = 0;
  char *word = calloc(WTREE_CHARS_NUMBER, sizeof(char));
  if (word == NULL)
    ERR(1, "Out of memory");
  unsigned int n = 0;
  memset(word, '\0', WTREE_CHARS_NUMBER);
  struct WTreeNode *node = wtree->root;
  get_wtree_word(node, word, &n, &size, result);
  free(word);
  return result;
}

static void destroy_node(struct WTreeNode *node) {
  free(node->chunk);
  if (node->size) {
    for (unsigned short int i = 0; i < node->size; i++)
      destroy_node(node->children.nodes[i]);
    free(node->children.nodes);
  }
  free(node);
}

void destroy_wtree(struct WTree *wtree) {
  destroy_node(wtree->root);
  free(wtree);
}
