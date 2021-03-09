#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex_node.h"

Node* create_node_with_label(const char *label, NodeType type) {
  // Possibly do some validation on label
  return create_node(NULL, label, type);
}

Node* create_node_with_lex(LexValue lex_value, NodeType type) {
  LexValue *value = (LexValue*) malloc(sizeof(LexValue));
  value = memcpy(value, &lex_value, sizeof(LexValue));
  
  char *label_tmp = get_label(lex_value);
  Node* node = create_node(value, label_tmp, type);
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
Node* create_id_node(LexValue data) {
  return create_node_with_lex(data, AST_IDENTIFIER);
}

Node* create_vector_node(Node* id, Node* expression) {
  Node* vector = create_node_with_label("[]", AST_VECTOR);
  
  append_child(vector,id);
  append_child(vector, expression);

  return vector;
}

Node* create_local_node(Node *local, Node *list) {
  if (local == NULL) {
    return list;
  }

  append_child(local, list);
  return local;
}

Node* create_io_node(Node* child, const char *label) {
  Node* output_node = create_node(NULL, label, AST_IO);
  append_child(output_node,child);
  
  return output_node;
}


Node* create_shift_node(LexValue shift, Node *id, Node* literal){
  Node* shift_node = create_node_with_lex(shift, AST_SHIFT);
  append_child(shift_node, id);
  append_child(shift_node, literal);
}


char* integerToString(int a){
  //Minimum integer: -2147483646 = 12 digitos + 1 para o \n
  int maxDigits = 12 + 1;
  
  char* integerInString = (char*) malloc(12 * sizeof(char));
  sprintf(integerInString,"%d",a);
  
  return integerInString;
}

Node* create_binary_tree(const char* parentLabel, NodeType type, Node *leftChild, Node *rightChild) {
  Node *parent = create_node_with_label(parentLabel, type);
  append_child(parent, leftChild);
  append_child(parent, rightChild);

  return parent;
}

Node* create_binary_exp(Node *parent, Node *leftChild, Node *rightChild) {
  append_child(parent, leftChild);
  append_child(parent, rightChild);

  return parent;
}

Node* create_ternary_node(Node *leftChild, Node *middleChild, Node *rightChild) {
  Node *parent = create_node_with_label("?:", AST_TERNARY);
  append_child(parent, leftChild);
  append_child(parent, middleChild);
  append_child(parent, rightChild);

  return parent;
}
Node* create_func_call_node(LexValue function_id, Node* expression){
  char* id = get_label(function_id);
  char* call = "call ";
  char* callPlusId = (char*) malloc(sizeof(char) * (strlen(call) + strlen(id) + 1));
  strcpy(callPlusId, call);
  strcat(callPlusId, id);
  
  Node* function_call = create_node_with_label(callPlusId, AST_FUNC_CALL);
  append_child(function_call, expression);

  free(callPlusId);
  free(function_id.token_value.s_value);
  free(id);

  return function_call;
}

Node* create_while_node(Node* expressao, Node* command){
  Node* whileNode = create_node_with_label("while", AST_WHILE);
  append_child(whileNode, expressao);
  append_child(whileNode, command);

  return whileNode;
}

Node* create_partial_if_node(Node* expressao, Node* command){
  Node* ifNode = create_node_with_label("if", AST_IF);
  append_child(ifNode, expressao);
  append_child(ifNode, command);

  return ifNode;
}

Node* create_for_node(Node* init, Node* expressao, Node* atualizacao, Node* command){
  Node* forNode = create_node_with_label("for", AST_FOR);
  append_child(forNode, init);
  append_child(forNode, expressao);
  append_child(forNode, atualizacao);
  append_child(forNode, command);
  
  return forNode;
}

Node* join_local_with_commands(Node *local, Node *commands) {
  if (local == NULL) {
    return commands;
  }

  Node* local_init = local;
  while (local_init->children_amount == 3) {
    local_init = local_init->children[local_init->children_amount-1];
  }
  append_child(local_init, commands);
  return local;
}

Node* join_command_lists(Node *parent_list, Node *child_list) {
  if (parent_list == NULL) {
    return child_list;
  }
  
  Node* node = parent_list;
  while (node->children_amount == expected_children_amount(node->type)+1) {
    node = node->children[node->children_amount-1];
  }
  append_child(node, child_list);
  return parent_list;
}