#include "asm.h"
#include <stdio.h>

AsmInstruction* create_asm_instruction(
  const char* label, 
  const char* opcode, 
  const char* src, 
  const char* dst) {
  AsmInstruction* instruction = (AsmInstruction*) malloc(sizeof(AsmInstruction));

  if (instruction == NULL) {
    return NULL;
  }

  instruction->label = label != NULL ? strdup(label) : NULL;
  instruction->opcode = strdup(opcode);
  instruction->src = src != NULL ? strdup(src) : NULL;
  instruction->dst = dst != NULL ? strdup(dst) : dst;
  instruction->next = NULL;
  instruction->prev = NULL;
  
  return instruction;
}

AsmInstruction* concat_asm_instructions(AsmInstruction* instr1, AsmInstruction* instr2){
  if(instr1 == NULL)
    return instr2;

  if(instr2 == NULL)
    return instr1;

  AsmInstruction* instr1_temp = instr1;
  while(instr1_temp->next != NULL)
    instr1_temp = instr1_temp->next;

  instr1_temp->next = instr2;
  instr2->prev = instr1_temp;
  
  return instr1;
}

AsmInstruction* generate_asm_code(Instruction* iloc_code){
  AsmInstruction* head = NULL;
  
  while(iloc_code != NULL){
    AsmInstruction* new_asm = iloc_to_asm(iloc_code);
    head = concat_asm_instructions(head, new_asm);
    iloc_code = iloc_code->previous;
  }
}

AsmInstruction* iloc_to_asm(Instruction* iloc){
  if(strcmp(iloc->opcode, "loadI") == 0){
    AsmInstruction* asm_code = create_asm_instruction(NULL, 
      "movl", 
      x86_literal(iloc->operand1), 
      iloc->operand3);
      return asm_code;
  } else if(strcmp(iloc->opcode, "add") == 0){
    AsmInstruction* copy = create_asm_instruction(NULL, "movl", iloc->operand1, iloc->operand3);
    AsmInstruction* add = create_asm_instruction(NULL,"addl" ,iloc->operand2, iloc->operand3);
    concat_asm_instructions(copy, add);
    return copy;
  } else if (strcmp(iloc->opcode, "i2i") == 0){
    AsmInstruction* move = create_asm_instruction(NULL, "movl", iloc->operand1, iloc->operand3);
    return move;
  } else if(strcmp(iloc->opcode, "storeAI") == 0){
    AsmInstruction* move = create_asm_instruction(NULL, "movl", iloc->operand1, x86_offset(iloc->operand2, iloc->operand3));
    return move;
  }
  return NULL;
}

void print_asm_instruction(AsmInstruction* asm_code){
  if(asm_code == NULL){
    return;
  }

  printf("%s %s,%s\n", asm_code->opcode, asm_code->src, asm_code->dst);
  print_asm_instruction(asm_code->next);
}

char* x86_literal(char* iloc_literal){
  char* literal = (char*) malloc(strlen(iloc_literal) + 1);
  literal = strdup("$");
  literal = strcat(literal, iloc_literal);

  return literal;
}

char* x86_offset(char* iloc_reg, char* offset){
  char* x86Reg = x86_reg(iloc_reg);
  char* x86Offset = (char*) malloc(sizeof(char) * (strlen(x86_reg(iloc_reg)) + strlen("-()") + strlen(offset) + 1));

  sprintf(x86Offset, "-%s(%s)", offset, x86Reg);
  return x86Offset;
}

char* x86_reg(char* iloc_reg)
{
  if (strcmp("rfp", iloc_reg) == 0)
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
