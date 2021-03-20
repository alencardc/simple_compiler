#include "table_stack.h"

Table_Stack* push_new_scope(Table_Stack* scopes, char* id){
    Table_Stack* newScope = (Table_Stack*) malloc(sizeof(Table_Stack));

    newScope->id = strdup(id);
    newScope->table = init_symbol_table();
    
    newScope->next = scopes;
    
    return newScope;
}

Symbol_Entry** top_scope(const Table_Stack* scopes){
    if (scopes == NULL)
        return NULL;

    return scopes->table;
}

Symbol_Entry* search_all_scopes(const Table_Stack* scopes,char *key){
    Table_Stack* current_scope = scopes;
    while(current_scope != NULL){
        Symbol_Entry** current_symbol_table = current_scope->table;
        Symbol_Entry* entry_queried = get_entry_from_table(key, current_symbol_table);
        
        if(entry_queried != NULL){
            printf("\nFound at: %s\n", current_scope->id);
            return entry_queried;
        }
            
        current_scope = current_scope->next;
    }

    return NULL;
}