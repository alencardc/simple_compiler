#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>
#include "../symbols_table/symbols_table.h"

typedef struct sInstruction {
  char* opcode;
  char* operand1;
  char* operand2;
  char* operand3;
  struct sInstruction* previous;
} Instruction;

Instruction* create_instruction(
  const char* opcode, 
  const char* arg1, 
  const char* arg2, 
  const char* arg3,
  Instruction* prev
);

static char* get_new_register();
static char* get_new_label(); 

Instruction* create_instr_identifier(Symbol_Entry* entry);
Instruction* create_instr_literal(Symbol_Entry* entry);

void print_instruction(Instruction* i);

#endif //  __INSTRUCTION_H__