#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../ast/lex_value.h"

//If you change please put a PRIME NUMBER
#define TABLE_SIZE 2

typedef struct sArgumentList{
    char* id;
    TokenValueType type;
    struct sArgumentList* next;
} Argument_List;

typedef enum eSymbol_Nature {
  LITERAL,
  VAR,
  VECTOR,
  FUNCTION
} Symbol_Nature;

typedef struct sSymbol_Entry {
    char* key;
    int line_number;
    Symbol_Nature nature;
    TokenValueType type;
    int length;
    Argument_List* arg_list;
    TokenValue value;
    struct sSymbol_Entry* next;
} Symbol_Entry;


Symbol_Entry** init_symbol_table();

Symbol_Entry* create_symbol_entry(const char* key, 
    int line_number, 
    Symbol_Nature nature,
    TokenValueType type,
    int lenght,
    TokenValue token_value);

int hash(char *key);

int insert_entry_at_table(Symbol_Entry* entry, Symbol_Entry** table);

Symbol_Entry* get_entry_from_table(char* key,Symbol_Entry **table);

void free_arg_list(Argument_List* list);

void free_entry(Symbol_Entry *entry);

void free_table(Symbol_Entry **table);





