#include "symbols_table.h"

Symbol_Entry** init_symbol_table(){
    Symbol_Entry** table = malloc(sizeof(Symbol_Entry*) * TABLE_SIZE);

    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        table[i] = NULL;
    }

    if(table == NULL){
        printf("ERRO na hora de alocar tabela de símbolos.");
    }
    
    return table;
}

Symbol_Entry* create_symbol_entry(const char* key, 
    int line_number, 
    Symbol_Nature nature,
    TokenValueType type,
    int lenght,
    TokenValue token_value){
    printf("=====> Criado: %s\n", key);
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
    if(entry == NULL)
        return -1;

    if(table == NULL)
        return -1;

    if(entry->key == NULL)
        return -1;

    int index = hash(entry->key);

    //Hash collision
    if(table[index] != NULL){
        if (table[index]->next == NULL){
            table[index]->next = entry;
        }else{
            Symbol_Entry* currentEntry = table[index]->next;
            while(currentEntry->next != NULL){
                currentEntry = currentEntry->next;
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

void free_arg_list(Argument_List* list){
    if(list == NULL)
        return;

    free(list->id);
   
    free_arg_list(list->next);
   
    free(list);
    return;
}



void free_entry(Symbol_Entry *entry){
    if(entry == NULL)
        return;

    free(entry->key);
    
    const TokenValueType value_type = entry->type;
    if (value_type == CHAR_VAL || value_type == STRING_VAL) {
      free(entry->value.s_value);
    }
    
    free_arg_list(entry->arg_list);

    free_entry(entry->next);
    
    free(entry);
    return;
}

void free_table(Symbol_Entry **table){
    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        free_entry(table[i]);
    }

    free(table);
}

void print_table(Symbol_Entry **table){
    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        if(table[i] != NULL){
            print_entry(table[i]);
            Symbol_Entry* current_entry = table[i]->next;
            while(current_entry != NULL){
                print_entry(current_entry);
                current_entry = current_entry->next;
            }
        }
            
    }
}

void print_entry(const Symbol_Entry* entry){
    printf("-------\n");
    printf("Key: %s\n", entry->key);
    printf("Line number: %i\n", (entry->line_number));
    printf("Symbol nature: %i\n", (entry->nature));
    printf("Type: %i\n", (entry->type));
    printf("Value: %s\n", entry->value.s_value);
    printf("Size: %i bytes\n", entry->length);
}
