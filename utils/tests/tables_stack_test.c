#include "../symbols_table/table_stack.h"
#include <stdio.h>

void print_entry(const Symbol_Entry* entry);

int main(){
    Table_Stack* scopes = NULL;
    scopes = push_new_scope(scopes, "global");

    Symbol_Entry** table = top_scope(scopes);
    TokenValue newValue = (TokenValue) 23.2;
    Symbol_Entry* new_entry = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);
    Symbol_Entry* zezim = create_symbol_entry("zezim", 10, VAR, FLOAT_VAL, 1, newValue);
    insert_entry_at_table(new_entry,table);
    insert_entry_at_table(zezim, table);

    scopes = push_new_scope(scopes, "local");
    table = top_scope(scopes);
    Symbol_Entry* new_entry2 = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);
    Symbol_Entry* new_entry3 = create_symbol_entry("zequinha", 10, VAR, FLOAT_VAL, 1, newValue);
    insert_entry_at_table(new_entry2, table);
    insert_entry_at_table(new_entry3, table);

    print_entry(search_all_scopes(scopes, "nova key"));
    search_all_scopes(scopes, "nova keyy");
    printf("Procurando zequinha: ");
    search_all_scopes(scopes, "zequinha");

    printf("Procurando zezim: ");
    search_all_scopes(scopes, "zezim");
}

void print_entry(const Symbol_Entry* entry){
    printf("-------\n");
    printf("Key: %s\n", entry->key);
    printf("Line number: %i\n", (entry->line_number));
    printf("Symbol nature: %i\n", (entry->nature));
    printf("Type: %i\n", (entry->type));
    printf("Value: %f\n", entry->value.f_val);
}