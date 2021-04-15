#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdbool.h>
#include "../symbols_table/table_stack.h"
#include "../ast/ast.h"
//#include "../prod_node.h"

typedef struct sInstruction {
  char* opcode;
  char* operand1;
  char* operand2;
  char* operand3;
  struct sInstruction* previous;
} Instruction;

typedef struct sProdNode {
  Node* ast;
  Instruction* instr;
  char* temp;
} ProdNode;

Instruction* create_instruction(
  const char* opcode, 
  const char* arg1, 
  const char* arg2, 
  const char* arg3,
  Instruction* prev
);

static char* get_new_register();
static char* get_new_label(); 

Instruction* append_instruction(Instruction* head, Instruction* new_instr);

void create_instr_identifier(ProdNode* prod, Table_Stack* scopes);
void create_instr_literal(ProdNode* prod, Table_Stack* scopes);
Instruction* create_instr_unary(char** result_temp, const char* op, ProdNode operand);
Instruction* create_instr_binary(char** result_temp, const char* op, ProdNode left, ProdNode right);

void print_instruction(Instruction* i);
void print_iloc_code(Instruction* head);

#endif //  __INSTRUCTION_H__