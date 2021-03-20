#include "symbols_table.h"

Symbol_Entry** init_symbol_table(){
    Symbol_Entry** table = malloc(sizeof(Symbol_Entry*) * TABLE_SIZE);

    if(table == NULL){
        printf("ERRO na hora de alocar tabela de símbolos.");
    }
    
    return table;
}

Symbol_Entry* create_symbol_entry(char* key, 
    int line_number, 
    Symbol_Nature nature,
    TokenValueType type,
    int lenght,
    TokenValue token_value){
    
    Symbol_Entry* entry = malloc(sizeof(Symbol_Entry));

    if(entry == NULL){
        printf("ERRO na hora de alocar uma symbol_entry. Key: %s", key);
        return NULL;
    }

    entry->arg_list = NULL;
    entry->key = strdup(key);
    entry->length = lenght;
    entry->next = NULL;
    entry->value = token_value;
    entry->line_number = line_number;
    entry->nature = nature;
    entry->next = NULL;

    return entry;
}