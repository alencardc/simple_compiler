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
    entry->value = token_value;
    entry->line_number = line_number;
    entry->nature = nature;
    entry->next = NULL;
    entry->type = type;

    return entry;
}

int hash(char *key) {
  unsigned int value = 5381;

  int i;
  for(i=0; i < strlen(key); i++)
      value = ((value << 5) + value) + (int)key[i];

  return value % TABLE_SIZE;
}

int insert_entry_at_table(Symbol_Entry* entry, Symbol_Entry** table){
    int index = hash(entry->key);

    //Hash collision
    if(table[index] != NULL){
        if (table[index]->next == NULL){
            table[index]->next = entry;
        }else{
            Symbol_Entry* currentEntry = table[index]->next;
            while(currentEntry->next != NULL){
                currentEntry = currentEntry->next;
                printf("%i\n", currentEntry);
            }
            currentEntry->next = entry;
        }
    }else{
        table[index] = entry;
    }

    return index;
}

Symbol_Entry* get_entry_from_table(char* key,Symbol_Entry **table){
    int index = hash(key);

    if(table[index] == NULL)
        return NULL;

    if(strcmp(key,table[index]->key) == 0)
        return table[index];

    Symbol_Entry* current_entry;
    if(table[index]->next != NULL){
        current_entry = table[index]->next;

        if(strcmp(key,current_entry->key) == 0)
            return current_entry;

        while(current_entry->next != NULL){
            current_entry = current_entry->next;

            if(strcmp(key,current_entry->key) == 0)
                return current_entry;
        }
    }

    return NULL;
}
