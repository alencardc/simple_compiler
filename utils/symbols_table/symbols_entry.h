#include "table_stack.h"
#include "../ast/tree.h"

typedef struct sId_list{
    char* id;
    struct sId_list* next;
    int vector_size;
    int line_number;
}Id_List;

typedef struct sProd_val{
  Node* node;
} Prod_Val;

Id_List* append_id_list(Id_List* list, Id_List* toAppend);
Id_List* create_id_list(char* newId, int vectorSize, int lineNumber);
void print_id_list(Id_List* list);

Symbol_Entry* create_id_entry();
Symbol_Entry* create_local_entry(const char* key, int line, TokenValueType type);
int get_type_lenght(TokenValueType type);
