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
  struct sAsmInstruction* prev;
} AsmInstruction;



AsmInstruction* create_asm_instruction(const char* label, const char* opcode, const char* src, const char* dst);
AsmInstruction* concat_asm_instructions(AsmInstruction* instr1, AsmInstruction* instr2);

AsmInstruction* iloc_to_asm(Instruction* iloc, AsmInstruction* prev);
AsmInstruction* generate_asm_code(Instruction* iloc_code, Symbol_Entry** global_scope);


void print_asm_instructions(AsmInstruction* asm_code);
void print_asm_instruction(AsmInstruction* asm_code);
void print_init_asm_code();
void print_final_asm_code();

char* x86_literal(char* iloc_literal);
char* x86_reg(char* iloc_reg);
char* x86_offset(char* iloc_reg, char* offset);

void print_asm_globals_code(Symbol_Entry** global_scope);

AsmInstruction* create_asm_cmp_code(Instruction* iloc_cmp, const char* jmp_type);

#endif // __ASM_H__
