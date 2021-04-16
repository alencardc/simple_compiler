#include "instruction_factory.h"

static char* get_new_register() {
  static int register_id = 0;
  int label_size = 1 + 1; // Letter r or L + \0
  label_size += 1 + register_id/10; // Digits required to represent register_id

  char* label = (char*) malloc(label_size*sizeof(char));
  sprintf(label, "r%d", register_id);

  register_id += 1;
  return label;
}

static char* get_new_label() {
  static int label_id = 0;
  int label_size = 1 + 1; // Letter r or L + \0
  label_size += 1 + label_id/10; // Digits required to represent label_id

  char* label = (char*) malloc(label_size*sizeof(char));
  sprintf(label, "L%d", label_id);

  label_id += 1;
  return label;
}

void create_instr_identifier(Node* node, Table_Stack* scopes) {
  if (node == NULL || scopes == NULL)
    return;
  
  Symbol_Entry* entry = search_all_scopes(scopes, node->label);
  if (entry == NULL) {
    node->temp = NULL;
    node->instr = NULL;
    return;
  }

  char* r = get_new_register();
  node->temp = r;

  char offset[12];
  sprintf(offset, "%d", entry->offset);

  if (entry->global == true) {
    node->instr = create_instruction("loadAI", "rbss", offset, r, NULL);
  } else {
    node->instr = create_instruction("loadAI", "rfp", offset, r, NULL);
  }
}

void create_instr_literal(Node* node, Table_Stack* scopes) {
  if (node == NULL || scopes == NULL)
    return;
  
  Symbol_Entry* entry = search_all_scopes(scopes, node->label);
  if (entry == NULL) {
    node->temp = NULL;
    node->instr = NULL;
    return;
  }
  
  char* r = get_new_register();
  node->temp = r;
  
  switch (entry->type) {
    case INTEGER_VAL:
      node->instr = create_instruction("loadI", entry->key, NULL, r, NULL);
      break;
  }
}

void create_instr_unary(Node* op_node, Node* operand) {
  if (op_node == NULL || operand->temp == NULL) {
    return;
  }

  Instruction* instr;
  char* r = get_new_register();
  op_node->temp = r;
  if (op_node->label[0] == '-') {
    instr = create_instruction("rsubI", operand->temp, "0", op_node->temp, NULL);
    //free(operand->temp); operand->temp = NULL;
    op_node->instr = insert_instr_begin(operand->instr, instr);
  }
  else if (op_node->label[0] == '!') {
    
  }

}

void create_instr_binary(Node* op_node, Node* left, Node* right) {
  if (op_node == NULL || left->temp == NULL || right->temp == NULL) {
    return;
  }

  Instruction* instr = NULL;
  char* r = get_new_register();
  op_node->temp = r;
  if (op_node->label[0] == '+') {
    instr = create_instruction("add", left->temp, right->temp, op_node->temp, NULL);
  } else if (op_node->label[0] == '-') {
    instr = create_instruction("sub", left->temp, right->temp, op_node->temp, NULL);
  } else if (op_node->label[0] == '*') {
    instr = create_instruction("mult", left->temp, right->temp, op_node->temp, NULL);
  } else if (op_node->label[0] == '/') {
    instr = create_instruction("div", left->temp, right->temp, op_node->temp, NULL);
  }

  // Not sure if it is needed yet
  // if (instr == NULL) {
  //   free(op_node->temp);
  //   op_node->temp = NULL;
  //   op_node->temp = left->temp;
  //   free(right->temp);
  //   right->temp = NULL;
  // } else {
  //   free(left->temp); free(right->temp); 
  //   left->temp = NULL; right->temp = NULL;
  // }
  Instruction* list = insert_instr_begin(right->instr, instr);

  op_node->instr = concat_instructions(list, left->instr);
}