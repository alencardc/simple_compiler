#include "table_stack.h"
#include "symbols_entry.h"
#include "../ast/ast.h"

// Symbol_Entry* create_id_entry(Node *id){
//     TokenValue value = (TokenValue) 0;
//     Symbol_Entry* new_id_entry = create_symbol_entry(
//         id->label,
//         id->data->line_number,
//         VAR,
//         NULL,
//         NULL,
//         value
//         );
// }

Id_List* append_id_list(Id_List* list,char* newId, int vector_size){
    Id_List* new_id = malloc(sizeof(Id_List));
    new_id->id = strdup(newId);
    new_id->next = NULL;
    new_id->vector_size = vector_size;

    if(list == NULL)
        return new_id;
    
    Id_List* current_id = list;
    while(current_id->next != NULL){
        current_id = current_id->next;
    }

    current_id->next = new_id;

    return list;
}