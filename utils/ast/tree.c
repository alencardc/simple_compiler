#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Node* create_node(LexValue *data, const char *label) {
  Node *node;
  node = (Node*) malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }

  node->label = strdup(label);
  node->data = data;
  node->children_amount = 0;
  node->children = NULL;

  return node;
}

Node* append_child(Node *parent, Node *child) {
  if (parent == NULL || child == NULL) {
    return NULL;
  }

  parent->children_amount += 1;
  parent->children = realloc(parent->children, parent->children_amount * sizeof(Node));
  parent->children[parent->children_amount - 1] = child;

  return parent;
}

void free_node(Node *node) {
  if (node == NULL) {
    return;
  }

  // Free all sub-tree from children
  for (int i = 0; i < node->children_amount; i++) {
    free_node(node->children[i]);
  }

  if (node->data != NULL) {
    const TokenValueType value_type = node->data->value_type;
    const TokenType token_type = node->data->token_type;
    if (value_type == CHAR_VAL || value_type == STRING_VAL || token_type != TOKEN_LITERAL) {
      free(node->data->token_value.s_value);
    }
    free(node->data); 
  }

  free(node->label);
  free(node->children);
  free(node);
}

void export_labels(Node *root) {
  if (root == NULL) {
    return;
  }
  // Quebrar ou nao a linha?
  printf("%p [label=\"%s\"];\n", root, root->label);
  for (int i = 0; i < root->children_amount; i += 1) {
    export_labels(root->children[i]);
  }
}

void export_relations(Node *root) {
  if (root == NULL) {
    return;
  }

  for (int i = 0; i < root->children_amount; i += 1) {
    // Quebrar ou nao a linha?
    printf("%p, %p\n", root, root->children[i]);
    export_relations(root->children[i]);
  }
}

void exporta(void *arvore) {
  export_relations((Node*) arvore);
  printf("\n");
  export_labels((Node*) arvore);
}

void libera(void *arvore) {
  free_node((Node*)arvore);
}