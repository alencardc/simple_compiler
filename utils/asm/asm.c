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

  
  instruction->label = label != NULL ? strdup(label) : NULL;
  instruction->opcode = strdup(opcode);
  instruction->src = src != NULL ? strdup(src) : NULL;
  instruction->dst = dst != NULL ? strdup(dst) : dst;
  instruction->next = next;
  
  return instruction;
}

AsmInstruction* generate_asm_code(Instruction* iloc_code, Symbol_Entry** global_scope){
  print_asm_globals_code(global_scope);
  
  while(iloc_code != NULL){
    iloc_to_asm(iloc_code);
    iloc_code = iloc_code->previous;
  }
}

AsmInstruction* iloc_to_asm(Instruction* iloc){
 
  AsmInstruction* asm_code;
  if(strcmp(iloc->opcode, "loadI") == 0){
    asm_code = create_asm_instruction(NULL, 
      "movl", 
      x86_literal(iloc->operand1), 
      x86_reg(iloc->operand3), 
      NULL);

      print_asm_instruction(asm_code);
  }
  return NULL;
}

void print_asm_instruction(AsmInstruction* asm_code){
  printf("%s %s %s\n", asm_code->opcode, asm_code->src, asm_code->dst);
}

char* x86_literal(char* iloc_literal){
  char* literal = (char*) malloc(strlen(iloc_literal) + 1);
  literal = strdup("$");
  literal = strcat(literal, iloc_literal);

  return literal;
}

char* x86_reg(char* iloc_reg)
{
  if (strcmp("r0", iloc_reg) == 0)
  {
    return "%ebx";
  } else if (strcmp("r1", iloc_reg) == 0)
  {
    return "%ecx";
  } else if (strcmp("r2", iloc_reg) == 0)
  {
    return "%edx";
  } else if (strcmp("r3", iloc_reg) == 0)
  {
    return "%esi";
  } else if (strcmp("r4", iloc_reg) == 0)
  {
    return "%edi";
  }
  else if (strcmp("rfp", iloc_reg) == 0)
  {
    return "%rbp";
  }
  else if (strcmp("rsp", iloc_reg) == 0)
  {
    return "%rsp";
  }
  else if (strcmp("rbss", iloc_reg) == 0)
  {
    return "%rsp";
  }
  else
  {
    return NULL;
  }
};


void print_asm_globals_code(Symbol_Entry** global_scope) {
  bool isFirstGlobal = true;
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (global_scope[i] != NULL) {
      Symbol_Entry* entry = global_scope[i];
      while (entry != NULL) {
        if (entry->nature == VAR) {
          if (isFirstGlobal == true) {
            printf("  .text\n");
            isFirstGlobal = false;
          }
          printf("  .globl %s\n", entry->key);
          printf("  .data\n");
          printf("  .align 4\n"); // depends on machine word length (?)
          printf("  .type %s, @object\n", entry->key);
          printf("  .size %s, %d\n", entry->key, entry->length);
          printf("%s:\n", entry->key);
          printf("  .long 0\n"); // in the future this will depend on type
        }

        entry = entry->next;
      }
    }
  }
}