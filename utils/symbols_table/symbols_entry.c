#include "symbols_entry.h"
#include "../ast/ast.h"
#include "../check_error.h"

void inject_value_type_from_scopes(Node* node, Table_Stack* scopes){
  Symbol_Entry* entry = search_all_scopes(scopes, node->label);
  node->value_type = entry->type;
}

void inject_value_type(Node* node, TokenValueType type){
    node->value_type = type;
}

int get_number_of_args_from_node(Node* first_arg){
    if(first_arg == NULL)
        return 0;

    Node* current_arg = first_arg;
    int number_of_args = 1;

    while(current_arg->children_amount > expected_children_amount(current_arg->type)){
        number_of_args++;
        current_arg = current_arg->children[current_arg->children_amount - 1];
    }

    return number_of_args;
}

int get_number_of_args_from_list(Argument_List* list){
    int number_of_args = 0;

    while(list != NULL){
        number_of_args++;
        list = list->next;
    }

    return number_of_args;
}

Argument_List* create_arg_list_element(char* id, TokenValueType type){
    Argument_List* new_argument = malloc(sizeof(Argument_List));

    if(new_argument == NULL)
        return NULL;
    
    new_argument->id = strdup(id);
    new_argument->next = NULL;
    new_argument->type = type;

    return  new_argument;
}

Argument_List* append_arg_list(Argument_List* list, Argument_List* toAppend, int line){
    check_function_string_par(toAppend->type, line);

    if(list == NULL)
        return toAppend;
    
    Argument_List* current_arg = list;
    while(current_arg->next != NULL){
        current_arg = current_arg->next;
    }
    current_arg->next = toAppend;
    
    return list;
}

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

Symbol_Entry* create_global_entry(Id_List* id_list, TokenValueType type){
    Symbol_Nature nature = id_list->vector_size > 1 ? VECTOR : VAR;
    int size = get_type_lenght(type);
    size = id_list->vector_size * size;

    //When we create a string global, its value is always 0 because we never initialize it
    if(type == STRING_VAL)
        size = 0;

    Symbol_Entry* new_entry = create_symbol_entry(id_list->id, 
                                  id_list->line_number,
                                  nature,
                                  type,
                                  size,
                                  (TokenValue) "",
                                  true
                                  );
    return new_entry;
}

Symbol_Entry* create_function_entry(const char* key, Argument_List* arg_list, TokenValueType returnType, int line){
    Symbol_Entry* new_entry = create_symbol_entry(
        key,
        line,
        FUNCTION,
        returnType,
        get_type_lenght(returnType),
        (TokenValue) 0,
        false
    );

    new_entry->arg_list = arg_list;

    return new_entry;
}

Symbol_Entry* create_local_entry(const char* key, int line, TokenValueType type, Table_Stack* scopes) {
    Symbol_Entry* new_entry = create_symbol_entry(
        key, 
        line,
        VAR,
        type,
        get_type_lenght(type),
        (TokenValue) 0,
        false
    );
    
    //Offset calculation from rfp or rbss
    new_entry->offset = scopes->offset;
    scopes->offset += new_entry->length;
    new_entry->global = false;
    
    return new_entry;
}

Symbol_Entry* create_literal_entry(const char* key, TokenValue value, int line, TokenValueType type) {
    if(type == STRING_VAL || type == CHAR_VAL){
        value.s_value = strdup(value.s_value);
    }

    Symbol_Entry* new_entry = create_symbol_entry(
        key, 
        line,
        LITERAL,
        type,
        get_type_lenght(type),
        value,
        false
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

void insert_local_entry_from_list(Node* list, TokenValueType type, Table_Stack* scopes, int line) {
    Symbol_Entry** scope = top_scope(scopes);

    Node* node = list;
    while (node != NULL) {
        if (node->type == AST_ASSIGN) {
            Node* node_var = node->children[0];
            Node* node_value = node->children[1];
            
            Symbol_Entry* new_entry = create_local_entry(node_var->label, node_var->data->line_number, type, scopes);
            
            
            check_identifier_redeclared(scopes, node_var->label, line);
            if (node_value->type == AST_IDENTIFIER) {
                check_identifier_undeclared(scopes, node_value->label, 0);
                check_wrong_var(scopes, node_value->label, line); 

                Symbol_Entry* value_entry =  search_all_scopes(scopes, node_value->label);
                node_value->value_type = value_entry->type;
                new_entry->length = value_entry->length;
                check_for_local_init_type_error(new_entry, node_value, line);
                // check identifier type
            } else if(node_value->type == AST_VECTOR){
                Node* id = node_value->children[0];
                check_identifier_undeclared(scopes,id->label, 0);
                check_wrong_vector(scopes, id->label, line); 
                id->value_type = search_all_scopes(scopes, id->label)->type;
                check_for_local_init_type_error(new_entry, id, line);
            } else { // AST_LITERAL
                check_for_local_init_type_error(new_entry, node_value, line);
                Symbol_Entry* literal_entry = create_literal_entry(
                    node_value->label,
                    node_value->data->token_value,
                    node_value->data->line_number,
                    node_value->data->value_type);
                insert_entry_at_table(literal_entry, scope);

                if(literal_entry->type == STRING_VAL){
                    int length_multiplier = strlen(literal_entry->value.s_value);
                    new_entry->length = new_entry->length * length_multiplier;
                    literal_entry->length = literal_entry->length * length_multiplier;
                }
            }
            insert_entry_at_table(new_entry, scope);
        } else if (node->type == AST_IDENTIFIER) {
            check_identifier_redeclared(scopes, node->label, line);
            Symbol_Entry* new_entry = create_local_entry(node->label, node->data->line_number, type, scopes);
            if(new_entry->type == STRING_VAL){
                new_entry->length = 0; //Strings não inicializadas tem tamanho 0.
            }
            insert_entry_at_table(new_entry, scope);
        }

        if (node->children_amount > expected_children_amount(node->type)) {
            node = node->children[node->children_amount-1];
        } else {
            node = NULL;
        }
    }
}

void insert_arg_list_at_func_scope(char* function_id, Table_Stack* scopes){
    Symbol_Entry* func_entry = search_all_scopes(scopes, function_id);
    Argument_List* arg_list = func_entry->arg_list;

    Symbol_Entry** func_scope = top_scope(scopes);
    int i = 1;
    while(arg_list != NULL){
        Symbol_Entry* new_symbol_entry = create_symbol_entry(arg_list->id, 
                                                            func_entry->line_number, 
                                                            VAR,
                                                            arg_list->type,
                                                            get_type_lenght(arg_list->type),
                                                            (TokenValue) 0,
                                                            false
                                                            );
        check_arg_redeclared(scopes, arg_list->id, i, get_line_number());
        insert_entry_at_table(new_symbol_entry, func_scope);
        arg_list = arg_list->next;
        i++;
    }
}