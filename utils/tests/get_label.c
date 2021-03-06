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

  char *intLabel = get_label(lex_val);

  lex_val.value_type = FLOAT_VAL;
  lex_val.token_value.f_val = 321.21;
  char *floatLabel = get_label(lex_val);

  lex_val.value_type = BOOL_VAL;
  lex_val.token_value.b_val = false;
  char *falseLabel = get_label(lex_val);

  lex_val.value_type = BOOL_VAL;
  lex_val.token_value.b_val = true;
  char *trueLabel = get_label(lex_val);

  lex_val.value_type = CHAR_VAL;
  lex_val.token_value.s_value = "a";
  char *charLabel = get_label(lex_val);

  lex_val.value_type = STRING_VAL;
  lex_val.token_value.s_value = "Many many";
  char *stringLabel = get_label(lex_val);

  lex_val.token_type = TOKEN_IDENTIFIER;
  lex_val.value_type = NO_VAL;
  lex_val.token_value.s_value = "my_var123";
  char *idLabel = get_label(lex_val);

  lex_val.token_type = TOKEN_OPERATOR;
  lex_val.value_type = NO_VAL;
  lex_val.token_value.s_value = ">=";
  char *opLabel = get_label(lex_val);

  lex_val.token_type = TOKEN_SPECIAL;
  lex_val.value_type = NO_VAL;
  lex_val.token_value.s_value = "!";
  char *spLabel = get_label(lex_val);

  printf("123 -> %s\n", intLabel);
  printf("321.21 -> %s\n", floatLabel);
  printf("false -> %s\n", falseLabel);
  printf("true -> %s\n", trueLabel);
  printf("a -> %s\n", charLabel);
  printf("Many many -> %s\n", stringLabel);
  printf("my_var123 -> %s\n", idLabel);
  printf(">= -> %s\n", opLabel);
  printf("! -> %s\n", spLabel);

  free(intLabel); free(floatLabel); free(falseLabel);
  free(trueLabel); free(charLabel); free(stringLabel);
  free(idLabel); free(opLabel); free(spLabel);

  return 0;
}