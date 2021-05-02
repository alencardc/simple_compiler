#ifndef __ASM_H__
#define __ASM_H__

#include <stdlib.h>
#include <string.h>
#include "../symbols_table/symbols_table.h"
#include "../iloc/instruction.h"

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
AsmInstruction* generate_asm_code(Instruction* iloc_code, Symbol_Entry** global_scope);
AsmInstruction* iloc_to_asm(Instruction* iloc);

void print_asm_instruction(AsmInstruction* asm_code);

char* x86_literal(char* iloc_literal);
char* x86_reg(char* iloc_reg);

void print_asm_globals_code(Symbol_Entry** global_scope);

#endif // __ASM_H__