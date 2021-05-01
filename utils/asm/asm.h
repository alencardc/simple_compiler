
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


AsmInstruction* create_asm_instruction(const char* label, const char* opcode, const char* src, const char* dst, AsmInstruction* next);
AsmInstruction* generate_asm_code(Instruction* iloc_code);
AsmInstruction* iloc_to_asm(Instruction* iloc);

void print_asm_instruction(AsmInstruction* asm_code);

char* x86_literal(char* iloc_literal);
char* x86_reg(char* iloc_reg);