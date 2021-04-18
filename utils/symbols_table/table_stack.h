#ifndef __TABLE_STACK_H__
#define __TABLE_STACK_H__

#include "symbols_table.h"

typedef struct sTableStack{
    char* id;
    Symbol_Entry** table;
    int offset;
    struct sTableStack* next;
}Table_Stack;


Table_Stack* push_new_scope(Table_Stack* scopes, char* id, int offset);

Symbol_Entry** top_scope(const Table_Stack* scopes);

Table_Stack* pop_scope(Table_Stack *scopes);

void free_scopes(Table_Stack* scopes);

Symbol_Entry* search_all_scopes(Table_Stack* scopes, const char *key);
Symbol_Entry* search_deep_scope(Table_Stack* scopes, const char *key);

void set_top_scope_name(const char* id, Table_Stack* scopes);

void print_table_stack(Table_Stack* scopes);

void inject_offset(Table_Stack* scopes);

#endif // __TABLE_STACK_H__