#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>

typedef struct sPlaceholder {
  char** label_ref;
  struct sPlaceholder* next;
} Placeholder;

Placeholder* create_placeholder(char** label);
void free_placeholder(Placeholder* placeholder);
Placeholder* insert_placeholder(Placeholder* new_item, Placeholder* list);

// typedef struct sProdNode {
//   Node* ast;
//   Instruction* instr;
//   char* temp;
//   Placeholder* tl; // true list
//   Placeholder* fl; // false list
// } ProdNode;

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

void free_instruction(Instruction* instr);

Instruction* insert_instr_begin(Instruction* head, Instruction* new_instr);
Instruction* concat_instructions(Instruction* head, Instruction* new_instr);

void print_instruction(Instruction* i);
void print_iloc_code(Instruction* head);

#endif //  __INSTRUCTION_H__