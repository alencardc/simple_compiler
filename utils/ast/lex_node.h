#ifndef __LEX_NODE_H__
#define __LEX_NODE_H__

#include "tree.h"

Node* create_node_with_label(const char *label);

Node* create_node_with_lex(LexValue lex_value);

char* get_label(LexValue lex_value);

#endif // __LEX_NODE_H__