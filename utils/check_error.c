#include "../errors.h"
#include "check_error.h"

bool check_identifier_redeclared(Table_Stack* scopes, char* key){
  Symbol_Entry** scope = top_scope(scopes);

  Symbol_Entry* queryEntry = get_entry_from_table(key, scope);
  if(queryEntry != NULL){
    printf("Já declarado");
    exit(ERR_DECLARED);
  }
  return false;
}

bool check_type_cast(Node* node_cast, Node* node_test) {
  TokenValueType cast_type = node_cast->data->value_type;
  TokenValueType test_type = node_test->data->value_type;
  bool isCastTypeValid = cast_type == INTEGER_VAL || cast_type == FLOAT_VAL || cast_type == BOOL_VAL;
  bool isTestTypeValid = test_type == INTEGER_VAL || test_type == FLOAT_VAL || test_type == BOOL_VAL;
  if (isCastTypeValid == true && isTestTypeValid == true) {
    return false;
  }

  printf("Tipo errado");
  exit(ERR_WRONG_TYPE);
}

bool check_identifier_undeclared(Table_Stack* scopes, char* key) {
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry == NULL) {
    printf("Não declarado :/\n");
    exit(ERR_UNDECLARED);
  }

  return false;
}

bool check_wrong_vector(Table_Stack* scopes, char* key, int line){
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry != NULL && queryEntry->nature != VECTOR){
    errors_as_vector(queryEntry, line);
  }

  return false;
}

void errors_as_vector(Symbol_Entry* entry, int line){
  switch (entry->nature){
    case VAR: printf("[ERR_VARIABLE] Variable \"%s\" was used as a Vector at line %i.\n", entry->key, line);
      exit(ERR_VARIABLE);
      break;
    case FUNCTION: printf("[ERR_FUNCTION] Function \"%s\" was used as a Vector at line %i.\n", entry->key, line);
      exit(ERR_FUNCTION);
    default:
      break;
  }
}

bool check_wrong_var(Table_Stack* scopes, char* key, int line){
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry != NULL && queryEntry->nature != VAR){
    errors_as_var(queryEntry, line);
  }

  return false;
}

void errors_as_var(Symbol_Entry* entry, int line){
  switch (entry->nature){
    case VECTOR:  printf("[ERR_VECTOR] Vector \"%s\" was used as a Variable at line %i.\n", entry->key, line);
      exit(ERR_VECTOR);
      break;
    case FUNCTION:  printf("[ERR_FUNCTION] Function \"%s\" was used as a Variable at line %i.\n", entry->key, line);
      exit(ERR_FUNCTION);
    default:
      break;
  }
}

bool check_wrong_par_input(int line, const char* key, Table_Stack* scopes) {
  Symbol_Entry* entry = search_all_scopes(scopes, key);
  if (entry->type == INTEGER_VAL || entry->type == FLOAT_VAL) {
    return false;
  }

  const char* type_string = token_value_type_to_string(entry->type);
  printf("[ERR_WRONG_PAR_INPUT] \"input\" command at line %d expected a "
    "parameter of type INTEGER or FLOAT, but the value \"%s\" is of type %s\n",
    line, key, type_string);
  exit(ERR_WRONG_PAR_INPUT);
}

bool check_wrong_arg_size(Node* args, const char* key, Table_Stack* scopes, int line){
  Symbol_Entry* entry = search_all_scopes(scopes, key);
 
  int declaration_arg_size = get_number_of_args_from_list(entry->arg_list);
  int actual_arg_size = get_number_of_args_from_node(args);
 
  if(actual_arg_size > declaration_arg_size){
    printf("[ERR_EXCESS_ARGS] Function \"%s\" expecting %i arguments(s) but was called with %i at line %i.\n", 
            key, 
            declaration_arg_size, 
            actual_arg_size, 
            line);
    exit(ERR_EXCESS_ARGS);
  } else if(declaration_arg_size > actual_arg_size){
    printf("[ERR_MISSING_ARGS] Function \"%s\" expecting %i arguments(s) but was called with %i at line %i.\n", 
            key, 
            declaration_arg_size, 
            actual_arg_size, 
            line);
    exit(ERR_MISSING_ARGS);
  }
  return false;
}