#include "utils/wtree.h"

#include <stdlib.h>
#include <string.h>

#include "utils/debug.h"
#include "utils/dstrings.h"

// Either some endpoint or more nodes
union WTreeChildren {
  struct WTreeNode **nodes;
  void *endpoint;
};

struct WTreeNode {
  char *chunk;
  unsigned int chunk_length;
  struct WTreeNode *parent;
  // Size shouldn't be more than 128
  unsigned short int size;
  union WTreeChildren children;
};

struct WTree {
  struct WTreeNode *root;
  unsigned int size;
};

static void destroy_node(struct WTreeNode *node);

static struct WTreeNode *create_node(struct WTreeNode *parent, char *chunk,
                                     unsigned int chunk_length) {
  struct WTreeNode *result = calloc(1, sizeof(struct WTreeNode));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTreeNode){.parent = parent,
                               .chunk = chunk,
                               .chunk_length = chunk_length,
                               .size = 0,
                               .children.nodes = NULL};
  return result;
}

struct WTree *create_wtree() {
  struct WTree *result = calloc(1, sizeof(struct WTree));
  if (result == NULL)
    ERR(1, "Out of memory");
  *result = (struct WTree){.root = create_node(NULL, NULL, 0), .size = 0};
  return result;
}

static unsigned int get_new_child_ind(struct WTreeNode *node, char c) {
  // TODO: Debug flag
  if (node == NULL) {
    WARN("Attempt to access NULL node");
    return 0;
  }

  INFOF("Searching place in node %s, size %u for %i(%c)", node->chunk,
        node->size, c, c);

  // Edge case - empty node
  if (!node->size)
    return 0;

  // Binary search
  unsigned int i1 = 0;
  unsigned int i2 = (unsigned int)node->size - 1;
  unsigned int s = i2 - i1;
  unsigned int i = s / 2;

  INFOF("i1: %u, i2: %u, s: %u, i: %u, chunk: %c", i1, i2, s, i,
        node->children.nodes[i1]->chunk[0]);

  // Two edge cases
  if (node->children.nodes[i1]->chunk[0] > c)
    return i1;

  if (node->children.nodes[i2]->chunk[0] < c)
    return i2 + 1;

  while (1) {
    if (node->children.nodes[i]->chunk[0] > c)
      i2 = i;
    else
      i1 = i;

    s = i2 - i1;

    if (s < 2)
      return i2;

    i = i1 + s / 2;
  }
}

static struct WTreeNode *get_child(struct WTreeNode *node, char c) {
  // TODO: Debug flag
  if (node == NULL) {
    WARN("Attempt to access NULL node");
    return NULL;
  }
  INFOF("Seaching node %s, size %u for %c(%i)", node->chunk, node->size, c, c);

  // Edge case - empty node
  if (!node->size)
    return NULL;

  // Binary search
  unsigned int i1 = 0;
  unsigned int i2 = (unsigned int)node->size - 1;
  unsigned int s = i2 - i1;
  unsigned int i = s / 2;

  while (1) {
    if (node->children.nodes[i1]->chunk[0] == c)
      return node->children.nodes[i1];

    if (node->children.nodes[i2]->chunk[0] == c)
      return node->children.nodes[i2];

    // Nothing found
    if (s <= 1)
      return NULL;

    if (node->children.nodes[i]->chunk[0] > c)
      i2 = i;
    else
      i1 = i;

    s = i2 - i1;
    i = i1 + s / 2;
  }
}

// Creates new node (chunk) and appends it to parent
static struct WTreeNode *append_node(struct WTreeNode *parent, char *chunk,
                                     unsigned int chunk_length) {
  INFO2F("Appending node %s to %s", chunk, parent->chunk);
  struct WTreeNode *child = create_node(parent, chunk, chunk_length);
  unsigned int new_ind = get_new_child_ind(parent, chunk[0]);
  INFO2F("New index: %u", new_ind);
  parent->size++;
  parent->children.nodes = realloc(parent->children.nodes,
                                   parent->size * sizeof(struct WTreeNode *));

  if (parent->children.nodes == NULL)
    ERR(1, "Out of memory");

  unsigned int nodes_to_copy = parent->size - 1u - new_ind;
  if (nodes_to_copy)
    memcpy(&(parent->children.nodes[new_ind + 1]),
           &(parent->children.nodes[new_ind]),
           nodes_to_copy * sizeof(struct WTreeNode *));

  parent->children.nodes[new_ind] = child;
  return child;
}

// Creates new node by splitting existing node by n chars (first n goes to the
// head)
static void split_node(struct WTreeNode *node, unsigned int n) {
  INFO2F("Splitting node %s by char #%i", node->chunk, n);
  // Splitting word
  unsigned int l = node->chunk_length;
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
  struct WTreeNode *tail = create_node(node, tail_chunk, l - n);
  tail->size = node->size;
  tail->children = node->children;
  // Replacing head
  node->size = 1;
  node->chunk_length = n + 1;
  free(node->chunk);
  node->chunk = head_chunk;
  node->children.nodes = calloc(1, sizeof(struct WTreeNode *));
  if (node->children.nodes == NULL)
    ERR(1, "Out of memory");
  node->children.nodes[0] = tail;
}

// Joins node with his only child
static void join_nodes(struct WTreeNode *node) {
  INFOF("Joining %s", node->chunk);
  struct WTreeNode *child = node->children.nodes[0];
  INFOF("Child node: %s", child->chunk);
  node->chunk =
      realloc(node->chunk,
              (node->chunk_length + child->chunk_length - 1) * sizeof(char));
  if (node->chunk == NULL)
    ERR(1, "Out of memory");
  strcat(node->chunk, child->chunk);
  node->size = 0;
  free(node->children.nodes);
  INFOF("Join result %s", node->chunk);
  destroy_node(child);
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
    unsigned int l = next_node->chunk_length - 1;
    unsigned int matched = get_str_match(next_node->chunk, tail);
    INFOF("tl == %u, l == %u, matched == %u", tl, l, matched);
    // Matched all chars - wtree already has this word
    if (l == matched && tl == matched + 1)
      ERRF(1, "Attempt to add existing word '%s' to wtree", word);
    tail = trim_str(tail, matched);
    tl -= matched;
    if (matched < l || (matched == l && !next_node->size))
      split_node(next_node, matched);
    node = next_node;
    next_node = get_child(node, tail[0]);
  }
  // At some point we reach uncharted territory
  INFOF("Child not found, appending tail %s", tail);
  struct WTreeNode *appended = append_node(node, tail, tl);
  appended->children.endpoint = endpoint;
  wtree->size++;
  INFO2("Expand complete");
}

unsigned int get_wtree_size(struct WTree *wtree) { return wtree->size; }

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

void shrink_wtree(struct WTree *wtree, char *word) {
  INFOF("Shrinking by %s", word);
  struct WTreeNode *to_delete = search_wtree(wtree, word);
  if (to_delete == NULL)
    ERRF(1, "Word not found: %s", word);
  // Delete node from parent's array
  struct WTreeNode *parent = to_delete->parent;
  int i = 0, found = 0;
  while (i < parent->size) {
    parent->children.nodes[i] = parent->children.nodes[i + found];
    if (parent->children.nodes[i] == to_delete) {
      found = 1;
      parent->size--;
    } else
      i++;
  }
  destroy_node(to_delete);
  INFOF("New parent size: %u", parent->size);

  if (parent->size)
    parent->children.nodes = realloc(parent->children.nodes,
                                     parent->size * sizeof(struct WTreeNode *));
  else
    free(parent->children.nodes);
  if (parent->children.nodes == NULL && parent->parent != NULL)
    ERR(1, "Out of memory");
  // If not root and parent have 1 child end child is is endpoint
  if (parent->parent != NULL && parent->size == 1 &&
      parent->children.nodes[0]->size == 0) {
    INFO("Gotta join");
    join_nodes(parent);
  }
  wtree->size--;
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
    l = node->chunk_length - 1;
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
  // Edge case
  if (!wtree->size)
    return NULL;
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
