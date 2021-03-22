#include "../errors.h"
#include "check_error.h"

bool check_identifier_undeclared(Table_Stack* scopes, char* key) {
  Table_Stack* scope = scopes;

  while (scope != NULL) {
    Symbol_Entry* queryEntry = get_entry_from_table(key, scope->table);
    if(queryEntry != NULL){
      return false;
    }
    scope = scope->next;
  }

  printf("Não declarado");
  exit(ERR_UNDECLARED);
}

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