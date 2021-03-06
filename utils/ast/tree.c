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
  node->parent = NULL;
  node->next = NULL;
  node->prev = NULL;
  node->children = NULL;

  return node;
}

Node* append_child(Node *parent, Node *child) {
  if (parent == NULL || child == NULL) {
    return NULL;
  }
  
  child->parent = parent;

  if (parent->children == NULL) {
    parent->children = child;
    return child;
  }

  Node* sibling = parent->children;
  while (sibling->next != NULL) {
    sibling = sibling->next;
  }
  sibling->next = child;
  child->prev = sibling;

  return child;
}

bool is_root(Node *node) {
  return node != NULL && node->parent == NULL;
}

void free_node(Node *node) {
  if (node == NULL) {
    return;
  }

  if (is_root(node) == false) {
    // Unlink node from siblings and parent
    if (node->prev == NULL) {
      node->parent->children = node->next;
    } else {
      node->prev->next = node->next;

      if (node->next != NULL) {
        node->next->prev = node->prev;
      }
    }
  }

  node->next = NULL;
  node->prev = NULL;
  free_node_rec(node);
}

static void free_node_rec(Node *node) {
  if (node == NULL) {
    return;
  }

  if (node->children != NULL) {
    free_node_rec(node->children);
  } 

  if (node->next != NULL) {
    free_node_rec(node->next);
  } 

  free(node->label);
  free(node->data);
  free(node);
  node = NULL;
}


void exporta(void *arvore) {
  //todo
}

void libera(void *arvore) {
  free_node((Node*)arvore);
}