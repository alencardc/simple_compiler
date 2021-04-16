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
  if (op_node->label[0] == '-') {
    char* r = get_new_register();
    op_node->temp = r;
    instr = create_instruction("rsubI", operand->temp, "0", op_node->temp, NULL);
    //free(operand->temp); operand->temp = NULL;
    op_node->instr = insert_instr_begin(operand->instr, instr);
  }
  else if (op_node->label[0] == '!') {
    op_node->tl = operand->fl;
    op_node->fl = operand->tl;
    op_node->instr = operand->instr;
    // What about op_node->temp?
  }
  else {
    // Not implement yet, just concat instructions and lists
    op_node->tl = operand->tl;
    op_node->fl = operand->fl;
    op_node->instr = operand->instr;
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
  } 
  else if (op_node->label[0] == '-') {
    instr = create_instruction("sub", left->temp, right->temp, op_node->temp, NULL);
  } 
  else if (op_node->label[0] == '*') {
    instr = create_instruction("mult", left->temp, right->temp, op_node->temp, NULL);
  } 
  else if (op_node->label[0] == '/') {
    instr = create_instruction("div", left->temp, right->temp, op_node->temp, NULL);
  } 
  else if (op_node->label[0] == '>') {
    instr = create_instruction("cmp_GT", left->temp, right->temp, op_node->temp, NULL);
    instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
    op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
    op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
  } 
  else if (op_node->label[0] == '<') {
    instr = create_instruction("cmp_LT", left->temp, right->temp, op_node->temp, NULL);
    instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
    op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
    op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
  } 
  else if (strcmp(op_node->label, ">=") == 0) {
    instr = create_instruction("cmp_GE", left->temp, right->temp, op_node->temp, NULL);
    instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
    op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
    op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
  } 
  else if (strcmp(op_node->label, "<=") == 0) {
    instr = create_instruction("cmp_LE", left->temp, right->temp, op_node->temp, NULL);
    instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
    op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
    op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
  }
  else if (strcmp(op_node->label, "==") == 0) {
    instr = create_instruction("cmp_EQ", left->temp, right->temp, op_node->temp, NULL);
    instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
    op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
    op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
  }
  else if (strcmp(op_node->label, "!=") == 0) {
    instr = create_instruction("cmp_EQ", left->temp, right->temp, op_node->temp, NULL);
    instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
    op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
    op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
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
  Instruction* list = concat_instructions(instr, right->instr);

  op_node->instr = concat_instructions(list, left->instr);
}

void create_instr_if(Node* if_node, Node* exp, Node* block) {
  if (exp == NULL || block == NULL)
    return;

  char* true_label = get_new_label();
  char* false_label = get_new_label();

  backpatch(exp->tl, true_label);
  backpatch(exp->fl, false_label);

  Instruction* true_label_instr = create_label(true_label, NULL);
  Instruction* false_label_instr = create_label(false_label, NULL);
  //if_node->instr = concat_instructions(false_label_instr, true_label_instr);
  if_node->instr = concat_instructions(true_label_instr, exp->instr);
  if_node->instr = concat_instructions(block->instr, if_node->instr);
  if_node->instr = concat_instructions(false_label_instr, if_node->instr);

  free(true_label);
  free(false_label);
}