#include "instruction.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Instruction* create_instruction(
  const char* opcode, 
  const char* arg1, 
  const char* arg2, 
  const char* arg3,
  Instruction* prev
) {
  Instruction* instruction = (Instruction*) malloc(sizeof(Instruction));

  if (instruction == NULL) {
    return NULL;
  }

  instruction->opcode = strdup(opcode);
  instruction->operand1 = strdup(arg1);
  instruction->operand2 = strdup(arg2);
  instruction->operand3 = strdup(arg3);
  instruction->previous = prev;

  return instruction;
}

static const char* get_new_register() {
  static int register_id = 0;
  int label_size = 1 + 1; // Letter r or L + \0
  label_size += 1 + register_id/10; // Digits required to represent register_id

  char* label = (char*) malloc(label_size*sizeof(char));
  sprintf(label, "r%d", register_id);

  register_id += 1;
  return label;
}

static const char* get_new_label() {
  static int label_id = 0;
  int label_size = 1 + 1; // Letter r or L + \0
  label_size += 1 + label_id/10; // Digits required to represent label_id

  char* label = (char*) malloc(label_size*sizeof(char));
  sprintf(label, "L%d", label_id);

  label_id += 1;
  return label;
}
