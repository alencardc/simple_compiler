#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>

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

static const char* get_new_register();
static const char* get_new_label(); 


#endif //  __INSTRUCTION_H__