#include "table_stack.h"

typedef struct sId_list{
    char* id;
    struct sId_list* next;
    int vector_size;
}Id_List;

Id_List* append_id_list(Id_List* list,char* newId, int vector_size);