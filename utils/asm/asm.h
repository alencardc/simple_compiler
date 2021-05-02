
#include "../iloc/instruction.h"
#include "stdlib.h"
#include "string.h"

typedef struct sAsmInstruction{
  char* label;
  char* opcode;
  char* src;
  char* dst; 
  struct sAsmInstruction* next;
} AsmInstruction;


AsmInstruction* create_asm_instruction(
  const char* label, 
  const char* opcode, 
  const char* src, 
  const char* dst, 
  AsmInstruction* next
);

AsmInstruction* iloc_to_asm(Instruction* iloc);