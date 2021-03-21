#include "symbols_table.h"

typedef struct sTableStack{
    char* id;
    Symbol_Entry** table;
    struct sTableStack* next;
}Table_Stack;


Table_Stack* push_new_scope(Table_Stack* scopes, char* id);

Symbol_Entry** top_scope(const Table_Stack* scopes);

Table_Stack* pop_scope(Table_Stack *scopes);

void free_scopes(Table_Stack* scopes);

Symbol_Entry* search_all_scopes(const Table_Stack* scopes,char *key);

