#include <stdio.h>
#include <stdlib.h>
#include "../ast/ast.h"


int main() {
  LexValue lex_val = {
    .line_number=0, 
    .token_type=TOKEN_LITERAL, 
    .token_value.i_val=123,
    .value_type=INTEGER_VAL
  };
  printf("Test start\n");
  Node *node = create_node_with_lex(lex_val);

  printf("label: 123 -> %s\n", node->label);
  printf("value: 123 -> %d\n", node->data->token_value.i_val);

  free_node(node);

  return 0;
}