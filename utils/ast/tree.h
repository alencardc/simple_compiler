#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>
#include "lex_value.h"

typedef struct sNode {
  LexValue *data;
  char *label;
  int children_amount;
  struct sNode **children;
} Node;

Node* create_node(LexValue *data, const char *label);
Node* append_child(Node *parent, Node *child);

//bool is_root(Node *node);

void free_node(Node *node);
static void free_node_rec(Node *node);


void exporta(void *arvore);
void libera(void *arvore);

#endif // __TREE_H__