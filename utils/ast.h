#ifndef __AST_H__
#define __AST_H__

#include <stdbool.h>

typedef enum eTokenValueType {
  CHAR_VAL,
  STRING_VAL,
  INTEGER_VAL,
  FLOAT_VAL,
  BOOL_VAL,
  NO_VAL = -1,
} TokenValueType;

typedef union sTokenValue {
  int i_val;
  float f_val;
  bool b_val;
  char* s_value;
} TokenValue;

typedef enum eTokenType {
  TOKEN_LITERAL,
  TOKEN_IDENTIFIER,
} TokenType;

typedef struct sLexValue {
  int line_number;
  TokenType token_type;
  TokenValueType value_type;
  TokenValue token_value;
} LexValue;

typedef struct sNode {
  LexValue *data;
  char *label;
  struct sNode *parent;
  struct sNode *next;
  struct sNode *prev;
  struct sNode *children;
} Node;

LexValue create_lex_value(TokenType token_type, const char* text, TokenValueType val_type, int line);

Node* create_node(LexValue *data, const char *label);
Node* append_child(Node *parent, Node *child);

bool is_root(Node *node);

void free_node(Node *node);
static void free_node_rec(Node *node);


void exporta(void *arvore);
void libera(void *arvore);

#endif // __AST_H__