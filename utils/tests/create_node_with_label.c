#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"

int main() {
  LexValue lex_val = {
    .line_number=0, 
    .token_type=TOKEN_LITERAL, 
    .token_value.i_val=0,
    .value_type=INTEGER_VAL
  };

  Node* p = create_node_with_label("Parent");
  Node* c1 = create_node_with_label("Node 1");
  Node* c2 = create_node_with_label("Node 2");
  Node* c3 = create_node_with_label("Node 3");
  Node* c11 = create_node_with_label("Node 1.1");
  Node* c12 = create_node_with_label("Node 1.2");
  Node* c21 = create_node_with_label("Node 2.1");

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