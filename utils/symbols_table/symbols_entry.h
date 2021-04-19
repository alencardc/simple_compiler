#ifndef __SYMBOLS_ENTRY_H__
#define __SYMBOLS_ENTRY_H__

#include "table_stack.h"
#include "../ast/tree.h"

typedef struct sId_list{
    char* id;
    struct sId_list* next;
    int vector_size;
    int line_number;
}Id_List;

void inject_value_type_from_scopes(Node* node, Table_Stack* scopes);
void inject_value_type(Node* node, TokenValueType type);

Id_List* append_id_list(Id_List* list, Id_List* toAppend);
Id_List* create_id_list(char* newId, int vectorSize, int lineNumber);
void print_id_list(Id_List* list);
void free_id_list(Id_List* list);

Argument_List* create_arg_list_element(char* id, TokenValueType type);
Argument_List* append_arg_list(Argument_List* list, Argument_List* toAppend, int line);

Symbol_Entry* create_global_entry(Id_List* id_list, TokenValueType type);
Symbol_Entry* create_literal_entry(const char* key, TokenValue value, int line, TokenValueType type);
Symbol_Entry* create_function_entry(const char* key, Argument_List* arg_list, TokenValueType returnType, int line);
Symbol_Entry* create_local_entry(const char* key, int line, TokenValueType type);

int get_number_of_args_from_node(Node* first_arg);
int get_number_of_args_from_list(Argument_List* list);

int get_type_lenght(TokenValueType type);

void insert_arg_list_at_func_scope(char* function_id, Table_Stack* scopes);

// Local var
void insert_local_entry_from_list(Node* list, TokenValueType type, Table_Stack* scopes, int line);

#endif // __SYMBOLS_ENTRY_H__