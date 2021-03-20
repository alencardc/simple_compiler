#include <stdlib.h>
#include <string.h>
#include "../ast/lex_value.h"

typedef struct sArgumentList{
    char* id;
    TokenValueType type;
    sArgumentList* next;
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
} Symbol_Entry;




