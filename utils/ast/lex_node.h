#ifndef __LEX_NODE_H__
#define __LEX_NODE_H__

#include "tree.h"

extern int get_line_number();

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
Node* create_function_node(Node* identifier, Node* command_list);

Node* create_binary_tree(const char* parent_label, NodeType type, Node *left_child, Node *right_child);
Node* create_binary_exp(Node *parent, Node *left_child, Node *right_child);
Node* create_ternary_node(Node *left_child, Node *middle_child, Node *right_child);

Node* create_while_node(Node* expression, Node* command);
Node* create_if_node(Node* expression, Node* command);
Node* create_if_else_node(Node* if_node, Node* else_commands);
Node* create_for_node(Node* init, Node* expression, Node* update, Node* command);

Node* join_local_with_commands(Node *local, Node *commands);
Node* join_command_lists(Node *parent_list, Node *child_list);

Node* free_all_id_nodes(Node* root);

#endif // __LEX_NODE_H__