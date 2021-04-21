#ifndef __SYMBOLS_TABLE_H__
#define __SYMBOLS_TABLE_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
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
    int offset; // Offset from rfp or rbss
    bool global; // Is global or not
    struct sSymbol_Entry* next;
    int return_offset;
    char* function_label;
} Symbol_Entry;


Symbol_Entry** init_symbol_table();

Symbol_Entry* create_symbol_entry(const char* key, 
    int line_number, 
    Symbol_Nature nature,
    TokenValueType type,
    int lenght,
    TokenValue token_value,
    bool is_global);

int hash(const char *key);

int insert_entry_at_table(Symbol_Entry* entry, Symbol_Entry** table);

Symbol_Entry* get_entry_from_table(const char* key,Symbol_Entry **table);

void free_arg_list(Argument_List* list);

void free_entry(Symbol_Entry *entry);

void free_table(Symbol_Entry **table);

void print_table(Symbol_Entry **table);

void print_entry(const Symbol_Entry* entry);

void print_arg_list(Argument_List* list);

#endif // __SYMBOLS_TABLE_H__