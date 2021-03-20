#include "../symbols_table/symbols_table.h"
#include <stdio.h>

int main(){

    TokenValue newValue = (TokenValue) 23.2;
    Symbol_Entry* new_entry = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);
    Symbol_Entry* new_entry2 = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);
    Symbol_Entry* new_entry3 = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);
    
    printf("key: %s\n", new_entry->key);

    Symbol_Entry** table = init_symbol_table();
    int index = insert_entry_at_table(new_entry, table);
    int index2 = insert_entry_at_table(new_entry2, table);
    // int index3 = insert_entry_at_table(new_entry, table);
    printf("New entry inserted at: %i\n", index);

    Symbol_Entry* queried_entry;
    queried_entry = get_entry_from_table("nova key", table);
    printf("key: %s\n", queried_entry->key);

    queried_entry = queried_entry->next;
    printf("key: %s\n", queried_entry->key);
    printf("next: %i\n", queried_entry->next);

    int index3 = insert_entry_at_table(new_entry3, table);
    printf("next: %i", new_entry2->next);
    printf("next: %i", new_entry3->next);
    

    return 0;
}