#ifndef __LEX_NODE_H__
#define __LEX_NODE_H__

#include "tree.h"

Node* create_node_with_label(const char *label, NodeType type);

Node* create_node_with_lex(LexValue lex_value, NodeType type);

char* get_label(LexValue lex_value);

/*Funções específicas*/
Node* create_id_node(LexValue data);
Node* create_vector_node(Node* id, Node* expression);
Node* create_local_node(Node *list, Node *local);
Node* create_io_node(Node* child, const char *label);
Node* create_shift_node(LexValue shift, Node *id, Node* literal);
Node* create_func_call_node(LexValue function_id, Node* expression);
Node* create_while_node(Node* expressao, Node* command);
Node* create_partial_if_node(Node* expressao, Node* command);
Node* create_for_node(Node* init, Node* expressao, Node* atualizacao, Node* command);
Node* create_binary_tree(const char* parentLabel, NodeType type, Node *leftChild, Node *rightChild);
Node* create_binary_exp(Node *parent, Node *leftChild, Node *rightChild);
Node* create_ternary_node(Node *leftChild, Node *middleChild, Node *rightChild);
Node* join_local_with_commands(Node *local, Node *commands);
Node* join_command_lists(Node *parent_list, Node *child_list);

char* integerToString(int a);

#endif // __LEX_NODE_H__