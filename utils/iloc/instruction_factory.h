#ifndef __INSTRUCTION_FACTORY_H__
#define __INSTRUCTION_FACTORY_H__

#include "instruction.h"
#include "../ast/ast.h"
#include "../symbols_table/table_stack.h"
#include "../symbols_table/symbols_entry.h"

static char* get_new_register();
static char* get_new_label(); 

void create_instr_identifier(Node* node, Table_Stack* scopes);
void create_instr_literal(Node* node, Table_Stack* scopes);

void create_instr_from_local_list(Node* list, Table_Stack* scopes);

void create_instr_assignment(Node* head, Node* id, Table_Stack* scopes, Node* exp);
void create_instr_unary(Node* op_node, Node* operand);

void create_instr_binary(Node* op_node, Node* left, Node* right);
bool create_instr_arith_op(Node* op_node, Node* left, Node* right);
bool create_instr_rel_op(Node* op_node, Node* left, Node* right);
bool create_instr_log_op(Node* op_node, Node* left, Node* right);

void create_instr_if(Node* if_node, Node* exp, Node* block);
void create_instr_if_else(Node* if_else_node, Node* if_node, Node* block);
void create_instr_while(Node* while_node, Node* exp, Node* block);
void create_instr_for(Node* for_node, Node* assign, Node* exp, Node* inc, Node* block);

Instruction* create_start_function_code(char* function_id, Table_Stack* scopes);

Instruction* create_function_call_code(char* function_id, Table_Stack* scopes);

char* get_arithop_instr(const char* op);
char* get_relop_instr(const char* op);
char* get_logop_instr(const char* op);

void create_instr_return(Node* return_node, Node* exp, Table_Stack* scopes);

#endif // __INSTRUCTION_FACTORY_H__