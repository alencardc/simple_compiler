#include "asm.h"
#include <stdio.h>

AsmInstruction* create_asm_instruction(
  const char* label, 
  const char* opcode, 
  const char* src, 
  const char* dst,
  AsmInstruction* next
) {
  AsmInstruction* instruction = (AsmInstruction*) malloc(sizeof(AsmInstruction));

  if (instruction == NULL) {
    return NULL;
  }

  instruction->label = strdup(label);
  instruction->opcode = strdup(opcode);
  instruction->src = strdup(src);
  instruction->dst = strdup(dst);

  instruction->next = next;
  
  return instruction;
}

AsmInstruction* iloc_to_asm(Instruction* iloc){
    print_iloc_code(iloc);
    return NULL;
}