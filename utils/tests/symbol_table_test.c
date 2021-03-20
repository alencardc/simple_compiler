#include "../symbols_table/symbols_table.h"
#include <stdio.h>

int main(){

    TokenValue newValue = (TokenValue) 23.2;
    Symbol_Entry* new_entry = create_symbol_entry("nova key", 10, VAR, FLOAT_VAL, 1, newValue);

    printf("key: %s", new_entry->key);

    return 0;
}