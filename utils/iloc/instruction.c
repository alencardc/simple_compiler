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

Instruction* create_instr_identifier(Symbol_Entry* entry) {
  if (entry == NULL)
    return NULL;

  char* r = get_new_register();

  Instruction* instruction = NULL;
  char offset[12];
  sprintf(offset, "%d", entry->offset);

  if (entry->global == true) {
    instruction = create_instruction("loadAI", "rbss", offset, r, NULL);
  } else {
    instruction = create_instruction("loadAI", "rfp", offset, r, NULL);
  }

  free(r);
  return instruction;
}

Instruction* create_instr_literal(Symbol_Entry* entry) {
  if (entry == NULL)
    return NULL;
  
  char* r = get_new_register();

  Instruction* instruction = NULL;
  
  switch (entry->type) {
    case INTEGER_VAL:
      instruction = create_instruction("loadI", entry->key, NULL, r, NULL);
      break;
  }

  free(r);
  return instruction;
}

void print_instruction(Instruction* i) {
  if (strcmp("loadAI", i->opcode) == 0) {
    printf("loadAI %s %s => %s\n", i->operand1, i->operand2, i->operand3);
  } 
  else if (strcmp("loadI", i->opcode) == 0) {
    printf("loadAI %s => %s\n", i->operand1, i->operand3);
  }
}