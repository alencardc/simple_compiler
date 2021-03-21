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

Id_List* append_id_list(Id_List* list, Id_List* toAppend){
    if(list == NULL)
        return toAppend;
    
    Id_List* current_id = list;
    while(current_id->next != NULL){
        current_id = current_id->next;
    }

    current_id->next = toAppend;

    return list;
}

Id_List* create_id_list(char* newId, int vectorSize, int lineNumber){
    Id_List* new_id = malloc(sizeof(Id_List));
    new_id->id = strdup(newId);
    new_id->next = NULL;
    new_id->vector_size = vectorSize;
    new_id->line_number = lineNumber;

    return new_id;
}

void print_id_list(Id_List* list){
    while (list != NULL)
    {
        printf("%s[%i],",list->id, list->vector_size);
        list = list->next;
    }
}

Symbol_Entry* create_id_entry(Id_List* id_list, TokenValueType type){
    Symbol_Nature nature = id_list->vector_size > 1 ? VECTOR : VAR;
    int size = get_type_lenght(type);
    size = id_list->vector_size * size;

    Symbol_Entry* new_entry = create_symbol_entry(id_list->id, 
                                  id_list->line_number,
                                  VAR,
                                  FLOAT_VAL,
                                  size,
                                  (TokenValue) ""
                                  );
    return new_entry;
}

Symbol_Entry* create_local_entry(const char* key, int line, TokenValueType type) {
    Symbol_Entry* new_entry = create_symbol_entry(
        key, 
        line,
        VAR,
        type,
        get_type_lenght(type),
        (TokenValue) ""
    );
    return new_entry;
}

int get_type_lenght(TokenValueType type){
    switch (type)
    {
    case CHAR_VAL: return 1;
    case STRING_VAL: return 1;
    case INTEGER_VAL: return 4;
    case FLOAT_VAL: return 8;
    case BOOL_VAL: return 1;    
    default:
        break;
    }
}
