#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "instruction.h"

Instruction* create_instruction(
  const char* opcode, 
  const char* arg1, 
  const char* arg2, 
  const char* arg3,
  Instruction* prev
) {
  Instruction* instruction = (Instruction*) malloc(sizeof(Instruction));

  if (instruction == NULL || arg1 == NULL) {
    return NULL;
  }

  instruction->opcode = strdup(opcode);
  instruction->operand1 = arg1 != NULL ? strdup(arg1) : NULL;
  instruction->operand2 = arg2 != NULL ? strdup(arg2) : NULL;
  instruction->operand3 = arg3 != NULL ? strdup(arg3) : NULL;
  instruction->previous = prev;
  
  return instruction;
}

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

Instruction* append_instruction(Instruction* head, Instruction* new_instr) {
  if (head == NULL) 
    return new_instr;
  if (new_instr == NULL) 
    return head;
  
  Instruction* temp = head;
  while (temp->previous != NULL) 
    temp = temp->previous;
  
  temp->previous = new_instr;
  return head;
}

void create_instr_identifier(ProdNode* prod, Table_Stack* scopes) {
  if (prod == NULL || scopes == NULL)
    return;
  
  Symbol_Entry* entry = search_all_scopes(scopes, prod->ast->label);
  if (entry == NULL) {
    prod->temp = NULL;
    prod->instr = NULL;
    return;
  }

  char* r = get_new_register();
  prod->temp = r;

  char offset[12];
  sprintf(offset, "%d", entry->offset);

  if (entry->global == true) {
    prod->instr = create_instruction("loadAI", "rbss", offset, r, NULL);
  } else {
    prod->instr = create_instruction("loadAI", "rfp", offset, r, NULL);
  }
}

void create_instr_literal(ProdNode* prod, Table_Stack* scopes) {
  if (prod == NULL || scopes == NULL)
    return;
  
  Symbol_Entry* entry = search_all_scopes(scopes, prod->ast->label);
  if (entry == NULL) {
    prod->temp = NULL;
    prod->instr = NULL;
    return;
  }
  
  char* r = get_new_register();
  prod->temp = r;
  
  switch (entry->type) {
    case INTEGER_VAL:
      prod->instr = create_instruction("loadI", entry->key, NULL, r, NULL);
      break;
  }
}

Instruction* create_instr_unary(char** result_temp, const char* op, ProdNode operand) {
  if (op == NULL || operand.temp == NULL) {
    return NULL;
  }

  Instruction* instr;
  char* r = get_new_register();
  *result_temp = r;
  if (op[0] == '-') {
    instr = create_instruction("rsubI", operand.temp, "0", *result_temp, NULL);
    free(operand.temp);
    return append_instruction(operand.instr, instr);
  }

  return NULL;
}

Instruction* create_instr_binary(char** result_temp, const char* op, ProdNode left, ProdNode right) {
  if (op == NULL || left.temp == NULL || right.temp == NULL) {
    return NULL;
  }

  Instruction* instr = NULL;
  char* r = get_new_register();
  *result_temp = r;
  if (op[0] == '+') {
    instr = create_instruction("add", left.temp, right.temp, *result_temp, NULL);
  } else if (op[0] == '-') {
    instr = create_instruction("sub", left.temp, right.temp, *result_temp, NULL);
  } else if (op[0] == '*') {
    instr = create_instruction("mult", left.temp, right.temp, *result_temp, NULL);
  } else if (op[0] == '/') {
    instr = create_instruction("div", left.temp, right.temp, *result_temp, NULL);
  }

  if (instr == NULL) {
    free(*result_temp);
    *result_temp = left.temp;
    free(right.temp);
  } else {
    free(left.temp); free(right.temp); 
  }
  Instruction* list = append_instruction(right.instr, instr);
  return append_instruction(left.instr, list);
}

void print_instruction(Instruction* i) {
  if (i == NULL)
    return;

  if (strcmp("loadAI", i->opcode) == 0) {
    printf("loadAI %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  } 
  else if (strcmp("loadI", i->opcode) == 0) {
    printf("loadI %s => %s\n", i->operand1, i->operand3);
  }
  else if (strcmp("rsubI", i->opcode) == 0) {
    printf("rsubI %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("add", i->opcode) == 0) {
    printf("add %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("sub", i->opcode) == 0) {
    printf("sub %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("mult", i->opcode) == 0) {
    printf("mult %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("div", i->opcode) == 0) {
    printf("div %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
}

void print_iloc_code(Instruction* head) {
  
  Instruction* instr = head;
  while (instr != NULL) {
    print_instruction(instr);
    instr = instr->previous;
  }
}