#ifndef __TREE_H__
#define __TREE_H__

#include <stdbool.h>
#include "lex_value.h"
#include "../iloc/instruction.h"

typedef enum sNodeType {
  AST_IDENTIFIER,
  AST_VECTOR,
  AST_IF,
  AST_IF_ELSE,
  AST_SHIFT,
  AST_ASSIGN,
  AST_IO,
  AST_FOR,
  AST_WHILE,
  AST_FUNC_CALL,
  AST_FUNC_DECL,
  AST_CONTROL,
  AST_RETURN,
  AST_LITERAL,
  AST_UNARY_EXP,
  AST_BINARY_EXP,
  AST_TERNARY,
} NodeType;

typedef struct sNode {
  // AST fields
  LexValue *data;
  char *label;
  NodeType type;
  int children_amount;
  struct sNode **children;
  TokenValueType value_type;

  // ILOC fields
  Instruction* instr;
  char* temp;
  Placeholder* tl; // true list
  Placeholder* fl; // false list
} Node;

Node* create_node(LexValue *data, const char *label, NodeType type);
Node* append_child(Node *parent, Node *child);

//bool is_root(Node *node);

void free_node(Node *node);

bool free_last_child_and_merge(Node* parent);
int expected_children_amount(NodeType type);

void print_debug_node(Node *root);
void export_labels(Node *root);
void export_relations(Node *root);

void exporta(void *arvore);
void libera(void *arvore);

#endif // __TREE_H__