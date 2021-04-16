#ifndef __INSTRUCTION_FACTORY_H__
#define __INSTRUCTION_FACTORY_H__

#include "instruction.h"
#include "../ast/ast.h"
#include "../symbols_table/table_stack.h"

static char* get_new_register();
static char* get_new_label(); 

void create_instr_identifier(Node* node, Table_Stack* scopes);
void create_instr_literal(Node* node, Table_Stack* scopes);
void create_instr_unary(Node* op_node, Node* operand);
void create_instr_binary(Node* op_node, Node* left, Node* right);
void create_instr_if(Node* if_node, Node* exp, Node* block);

#endif // __INSTRUCTION_FACTORY_H__