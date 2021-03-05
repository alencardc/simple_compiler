#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>

typedef struct sValue {
  int i_value;
  float f_value;
} Value;

typedef struct sNode {
  Value *data;
  struct sNode *parent;
  struct sNode *next;
  struct sNode *prev;
  struct sNode *children;
} Node;

Node* create_node(Value *data);
Node* append_child(Node *parent, Node *child);

bool is_root(Node *node);

void free_node(Node *node);
static void free_node_rec(Node *node);


void exporta(void *root);
void libera(void *root);

#endif // __TREE_H__