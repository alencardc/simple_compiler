#include "../symbols_table/symbols_table.h"
#include <stdio.h>

void print_entry(const Symbol_Entry* entry);

int main(){

    TokenValue newValue = (TokenValue) 23.2;
    Symbol_Entry* new_entry = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);
    Symbol_Entry* new_entry2 = create_symbol_entry("nova key2", 20, VAR, FLOAT_VAL, 1, newValue);
    Symbol_Entry* new_entry3 = create_symbol_entry("nova key3", 10, FUNCTION, FLOAT_VAL, 1, newValue);
    
    printf("key: %s\n", new_entry->key);

    Symbol_Entry** table = init_symbol_table();
    int index = insert_entry_at_table(new_entry, table);
    int index2 = insert_entry_at_table(new_entry2, table);
    int index3 = insert_entry_at_table(new_entry3, table);
    // int index3 = insert_entry_at_table(new_entry, table);
    printf("New entry inserted at: %i\n", index);

    Symbol_Entry* queried_entry;
    queried_entry = get_entry_from_table("nova key2", table);
    print_entry(queried_entry);

    queried_entry = get_entry_from_table("nova key3", table);
    print_entry(queried_entry);

    return 0;
}

void print_entry(const Symbol_Entry* entry){
    printf("-------\n");
    printf("Key: %s\n", entry->key);
    printf("Line number: %i\n", (entry->line_number));
    printf("Symbol nature: %i\n", (entry->nature));
    printf("Type: %i\n", (entry->type));
    printf("Value: %f\n", entry->value.f_val);
}