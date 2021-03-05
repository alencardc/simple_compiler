#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>

typedef struct sValue {
  int i_value;
  float f_value;
} Value;

typedef struct sNode {
  Value *data;
  char *label;
  struct sNode *parent;
  struct sNode *next;
  struct sNode *prev;
  struct sNode *children;
} Node;

Node* create_node(Value *data, const char *label);
Node* append_child(Node *parent, Node *child);

bool is_root(Node *node);

void free_node(Node *node);
static void free_node_rec(Node *node);


void exporta(void *arvore);
void libera(void *arvore);

#endif // __TREE_H__