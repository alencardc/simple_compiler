#include "table_stack.h"

Table_Stack* push_new_scope(Table_Stack* scopes, char* id, int offset){
    Table_Stack* newScope = (Table_Stack*) malloc(sizeof(Table_Stack));

    newScope->id = strdup(id);
    newScope->table = init_symbol_table();
    newScope->offset = offset;
    
    newScope->next = scopes;
    
    return newScope;
}

Symbol_Entry** top_scope(const Table_Stack* scopes){
    if (scopes == NULL)
        return NULL;

    return scopes->table;
}

Symbol_Entry* search_all_scopes(Table_Stack* scopes, const char *key){
    Table_Stack* current_scope = scopes;
    while(current_scope != NULL){
        Symbol_Entry** current_symbol_table = current_scope->table;
        Symbol_Entry* entry_queried = get_entry_from_table(key, current_symbol_table);
        
        if(entry_queried != NULL){
            return entry_queried;
        }
            
        current_scope = current_scope->next;
    }

    return NULL;
}

Symbol_Entry* search_deep_scope(Table_Stack* scopes, const char *key){
    if(scopes == NULL)
        return NULL;
    
    Table_Stack* current_scope = scopes;

    while(current_scope->next != NULL){
        current_scope = current_scope->next;
    }

    Symbol_Entry** table = current_scope->table;
    Symbol_Entry* queried = get_entry_from_table(key, table);

    return queried;
}

Table_Stack* pop_scope(Table_Stack *scopes){
    if(scopes == NULL)
        return NULL;
        
    free(scopes->id);
    free_table(scopes->table);

    Table_Stack* toReturn = scopes->next;
    free(scopes);
    
    return toReturn;
}

void free_scopes(Table_Stack* scopes){
    if(scopes == NULL)
        return;
    
    Table_Stack* current_scope = pop_scope(scopes);

    while(current_scope != NULL){
        current_scope = pop_scope(current_scope);
    }
}

void set_top_scope_name(const char* id, Table_Stack* scopes) {
    free(scopes->id);
    scopes->id = strdup(id);
}

void print_table_stack(Table_Stack* scopes) {
    Table_Stack* scope = scopes;
    while (scope != NULL) {
        printf("\n\nTable/scope: %s\n", scope->id);
        print_table(scope->table);
        scope = scope->next;
    }
}

void inject_offset(Table_Stack* scopes){
    Table_Stack* to_inject_scope = scopes->next;

    to_inject_scope->offset = scopes->offset;
    printf("Escopo que vai ser injetado: %i", scopes->offset);
}