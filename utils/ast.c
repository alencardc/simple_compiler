#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

LexValue create_lex_value(TokenType token_type, TokenValue value, TokenValueType val_type, int line) {

  return (LexValue){
    .line_number = line,
    .token_type = token_type,
    .value_type = val_type,
    .token_value = value,
  };
}

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

  return parent;
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
  free(node);
  node = NULL;
}


void exporta(void *arvore) {
  //todo
}

void libera(void *arvore) {
  free_node((Node*)arvore);
}

/*Funções específicas*/

Node* create_io_node(Node* child, const char *label) {
  Node* output_node = create_node(NULL, label);
  append_child(output_node,child);

  return output_node;
}

Node* create_id_node(LexValue data){
  Node* id_node = create_node(&data, "identificador");
  return id_node;
}

Node* create_shift_node(LexValue shift, Node *id, Node* literal){
  Node* shift_node = create_node(&shift,shift.token_value.s_value);
  
  append_child(shift_node, id);
  append_child(shift_node, literal);
}

Node* create_id_vector_node(Node* id, Node* expression){
  Node* id_vector = create_node(NULL, "[]");
  
  append_child(id_vector,id);
  append_child(id_vector, expression);

  return id_vector;
}

char* integerToString(int a){
  //Minimum integer: -2147483646 = 12 digitos + 1 para o \n
  int maxDigits = 12 + 1;
  
  char* integerInString = (char*) malloc(12 * sizeof(char));
  sprintf(integerInString,"%d",a);
  
  return integerInString;
}
