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

/*Funções específicas*/

Node* create_io_node(Node* child, const char *label) {
  Node* output_node = create_node(NULL, label);
  append_child(output_node,child);

  return output_node;
}

Node* create_id_node(LexValue data){
  Node* id_node = create_node(&data, "identificador");
  return id_node;
}

Node* create_shift_node(LexValue shift, Node *id, Node* literal){
  Node* shift_node = create_node(&shift,shift.token_value.s_value);
  append_child(shift_node, id);
  append_child(shift_node, literal);
}

Node* create_id_vector_node(Node* id, Node* expression){
  Node* id_vector = create_node(NULL, "[]");
  
  append_child(id_vector,id);
  append_child(id_vector, expression);

  return id_vector;
}

char* integerToString(int a){
  //Minimum integer: -2147483646 = 12 digitos + 1 para o \n
  int maxDigits = 12 + 1;
  
  char* integerInString = (char*) malloc(12 * sizeof(char));
  sprintf(integerInString,"%d",a);
  
  return integerInString;
}

Node* create_binary_tree(const char* parentLabel, Node *leftChild, Node *rightChild) {
  Node *parent = create_node_with_label(parentLabel);
  append_child(parent, leftChild);
  append_child(parent, rightChild);

  return parent;
}

Node* create_binary_exp(Node *parent, Node *leftChild, Node *rightChild) {
  append_child(parent, leftChild);
  append_child(parent, rightChild);

  return parent;
}

Node* create_ternary_tree(const char* parentLabel, Node *leftChild, Node *middleChild, Node *rightChild) {
  Node *parent = create_node_with_label(parentLabel);
  append_child(parent, leftChild);
  append_child(parent, middleChild);
  append_child(parent, rightChild);

  return parent;
}
Node* create_func_call_node(LexValue function_id, Node* expression){
  char* id = get_label(function_id);
  char* call = "call ";
  char* callPlusId = (char*) malloc(sizeof(char) * (strlen(call) + strlen(id)));
  strcpy(callPlusId, call);
  strcat(callPlusId, id);
  
  Node* function_call = create_node_with_label(callPlusId);
  append_child(function_call, expression);
  return function_call;
}

Node* create_while_node(Node* expressao, Node* command){
  Node* whileNode = create_node_with_label("while");
  append_child(whileNode, expressao);
  append_child(whileNode, command);

  return whileNode;
}

Node* create_partial_if_node(Node* expressao, Node* command){
  Node* ifNode = create_node_with_label("if");
  append_child(ifNode, expressao);
  append_child(ifNode, command);

  return ifNode;
}

Node* create_for_node(Node* init, Node* expressao, Node* atualizacao, Node* command){
  Node* forNode = create_node_with_label("for");
  append_child(forNode, init);
  append_child(forNode, expressao);
  append_child(forNode, atualizacao);
  append_child(forNode, command);
  
  return forNode;
}

