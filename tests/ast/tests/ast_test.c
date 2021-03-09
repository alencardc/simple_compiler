#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"

LexValue* copy(LexValue val) {
  LexValue* lex = (LexValue*) malloc(sizeof(LexValue));
  memcpy(lex, &val, sizeof(LexValue));
  lex->token_value.s_value = strdup(lex->token_value.s_value);

  return lex;
}

int main() {
  LexValue lex_val = {
    .line_number=0, 
    .token_type=TOKEN_IDENTIFIER, 
    .token_value.s_value="aa",
    .value_type=NO_VAL
  };

  Node* p = create_node(copy(lex_val), "Parent");
  Node* c1 = create_node(copy(lex_val), "Node 1");
  Node* c2 = create_node(copy(lex_val), "Node 2");
  Node* c3 = create_node(copy(lex_val), "Node 3");
  Node* c11 = create_node(copy(lex_val), "Node 1.1");
  Node* c12 = create_node(copy(lex_val), "Node 1.2");
  Node* c21 = create_node(copy(lex_val), "Node 2.1");

  if (!p || !c1 || !c2 || !c3 || !c11 || !c12 || !c21) {
    printf("Error creating node\n");
  }

  append_child(p, c1);
  append_child(p, c2);
  append_child(p, c3);
  append_child(c1, c11);
  append_child(c1, c12);
  append_child(c2, c21);

  printf("Parent -> %s\n", p->label);
  printf("Child 1 -> %s\n", p->children[0]->label);
  printf("Child 2 -> %s\n", p->children[1]->label);
  printf("Child 3 -> %s\n", p->children[2]->label);
  printf("Child 11 -> %s\n", c1->children[0]->label);
  printf("Child 12 -> %s\n", c1->children[1]->label);
  printf("Child 21 -> %s\n", c2->children[0]->label);

  free_node(p);
  return 0;
}