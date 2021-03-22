#include "symbols_entry.h"
#include "../ast/ast.h"
#include "../check_error.h"

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

void free_id_list(Id_List* list){
    if(list == NULL)
        return;

    if(list->next != NULL)
        free_id_list(list->next);
    
    free(list->id);
    free(list);
    return;
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
                                  type,
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

Symbol_Entry* create_literal_entry(const char* key, TokenValue value, int line, TokenValueType type) {
    Symbol_Entry* new_entry = create_symbol_entry(
        key, 
        line,
        LITERAL,
        type,
        get_type_lenght(type),
        value
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

void insert_local_entry_from_list(Node* list, TokenValueType type, Table_Stack* scopes) {
    Symbol_Entry** scope = top_scope(scopes);

    Node* node = list;
    while (node != NULL) {
        if (node->type == AST_ASSIGN) {
            Node* node_var = node->children[0];
            Node* node_value = node->children[1];
            
            Symbol_Entry* new_entry = create_local_entry(node_var->label, node_var->data->line_number, type);

            check_identifier_redeclared(scopes, node_var->label);
            if (node_value->type == AST_IDENTIFIER || node_value->type == AST_VECTOR) {
                check_identifier_undeclared(scopes, node_value->label);

                // check identifier type
            } else { // AST_LITERAL
                //check_type_cast(node_var, node_value);
                Symbol_Entry* literal_entry = create_literal_entry(
                    node_value->label,
                    node_value->data->token_value,
                    node_value->data->line_number,
                    node_value->data->value_type);
                insert_entry_at_table(literal_entry, scope);
            }

            insert_entry_at_table(new_entry, scope);
        } else if (node->type == AST_IDENTIFIER) {
            check_identifier_redeclared(scopes, node->label);
            Symbol_Entry* new_entry = create_local_entry(node->label, node->data->line_number, type);
            insert_entry_at_table(new_entry, scope);
        }

        if (node->children_amount > expected_children_amount(node->type)) {
            node = node->children[node->children_amount-1];
        } else {
            node = NULL;
        }
    }
}