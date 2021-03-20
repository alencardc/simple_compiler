#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../ast/lex_value.h"

#define TABLE_SIZE 500

typedef struct sArgumentList{
    char* id;
    TokenValueType type;
    struct sArgumentList* next;
} Argument_List;

typedef enum eSymbol_Nature {
  LITERAL,
  VAR,
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

Symbol_Entry* create_symbol_entry(char* key, 
    int line_number, 
    Symbol_Nature nature,
    TokenValueType type,
    int lenght,
    TokenValue token_value);




