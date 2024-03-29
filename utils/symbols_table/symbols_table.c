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
    TokenValue token_value,
    bool is_global
    ){
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
    entry->offset = 0; // TODO
    entry->global = is_global; // TODO
    entry->return_offset = 0;
    entry->function_label = NULL;

    return entry;
}

int hash(const char *key) {
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

Symbol_Entry* get_entry_from_table(const char* key, Symbol_Entry **table){
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
    if (value_type == STRING_VAL || value_type == CHAR_VAL) {
        if(entry->nature == LITERAL){
            free(entry->value.s_value);
        }
    }
    
    free_arg_list(entry->arg_list);
    free(entry->function_label);

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
    printf("Key\t\tLine\tNature\tType\tValue\t\tSize\tArgument List\n");
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
    printf("%s\t\t", entry->key);
    printf("%i\t", entry->line_number);
    printf("%i\t", entry->nature);
    printf("%i\t", entry->type);
    if (entry->type == CHAR_VAL || entry->type == STRING_VAL) {
        printf("%s\t", entry->value.s_value);
    } else if (entry->type == BOOL_VAL || entry->type == INTEGER_VAL) {
        printf("%d\t", entry->value.i_val);
    } else if (entry->type == FLOAT_VAL) {
        printf("%f\t", entry->value.f_val);
    } else {
        printf("\t");
    }
    printf("\t%i\t", entry->length);

    print_arg_list(entry->arg_list);
}

void print_arg_list(Argument_List* list){
    while (list != NULL)
    {
        printf("%s(%i) ",list->id, list->type);
        list = list->next;
    }
    printf("\n");
}