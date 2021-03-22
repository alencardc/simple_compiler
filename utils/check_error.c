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

bool check_wrong_vector(Table_Stack* scopes, char* key){
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry != NULL && queryEntry->nature != VECTOR){
    errors_as_vector(queryEntry);
  }

  return false;
}

void errors_as_vector(Symbol_Entry* entry){
  switch (entry->nature){
    case VAR: printf("Isso era uma varíavel e foi usado como vetor");
      exit(ERR_VARIABLE);
      break;
    case FUNCTION: printf("Isso era uma função e foi usado como vetor");
      exit(ERR_FUNCTION);
    default:
      break;
  }
}

bool check_wrong_var(Table_Stack* scopes, char* key){
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry != NULL && queryEntry->nature != VAR){
    errors_as_var(queryEntry);
  }

  return false;
}

void errors_as_var(Symbol_Entry* entry){
  switch (entry->nature){
    case VECTOR: printf("Isso era uma vetor e foi usado como varíavel");
      exit(ERR_VECTOR);
      break;
    case FUNCTION: printf("Isso era uma função e foi usado como varíavel");
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