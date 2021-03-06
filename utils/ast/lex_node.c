#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_node.h"

Node* create_node_with_label(const char *label) {
  // Possibly do some validation on label
  return create_node(NULL, label);
}

Node* create_node_with_lex(LexValue lex_value) {
  LexValue *value = (LexValue*) malloc(sizeof(LexValue));
  value = memcpy(value, &lex_value, sizeof(LexValue));
  
  char *label_tmp = get_label(lex_value);
  Node* node = create_node(value, label_tmp);
  free(label_tmp);

  return node;
}

char* get_label(LexValue lex_value) {
  const int max_length = 51;
  char tmp_str[max_length];
  char *label;

  if (lex_value.token_type == TOKEN_LITERAL) {
    switch (lex_value.value_type) {
      case INTEGER_VAL:
        snprintf(tmp_str, max_length, "%d", lex_value.token_value.i_val);
        label = strdup(tmp_str);
        break;
      
      case FLOAT_VAL:
        snprintf(tmp_str, max_length, "%g", lex_value.token_value.f_val);
        label = strdup(tmp_str);
        break;

      case STRING_VAL:
      case CHAR_VAL:
        label = strdup(lex_value.token_value.s_value);
        break;

      case BOOL_VAL:
        label = strdup(lex_value.token_value.b_val == true ? "true" : "false");
        break;

      default: // NO_VAL
        break;
    }
  } else {
    // TOKEN_OPERATOR || TOKEN_IDENTIFIER || TOKEN_SPECIAL
    label = strdup(lex_value.token_value.s_value);
  }

  return label;
}