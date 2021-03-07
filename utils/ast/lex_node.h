#ifndef __LEX_NODE_H__
#define __LEX_NODE_H__

#include "tree.h"

Node* create_node_with_label(const char *label);

Node* create_node_with_lex(LexValue lex_value);

char* get_label(LexValue lex_value);

/*Funções específicas*/
Node* create_io_node(Node* child, const char *label);
Node* create_id_node(LexValue data);
Node* create_shift_node(LexValue shift, Node *id, Node* literal);
Node* create_id_vector_node(Node* id, Node* expression);
Node* create_binary_tree(const char* parentLabel, Node *leftChild, Node *rightChild);
Node* create_binary_exp(Node *parent, Node *leftChild, Node *rightChild);
Node* create_ternary_tree(const char* parentLabel, Node *leftChild, Node *middleChild, Node *rightChild);

char* integerToString(int a);

#endif // __LEX_NODE_H__