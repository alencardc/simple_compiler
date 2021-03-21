#include "../errors.h"
#include "check_error.h"

bool check_identifier() {}

bool check_identifier_redeclared(Table_Stack* scopes, char* key){
    Symbol_Entry** scope = top_scope(scopes);

     Symbol_Entry* queryEntry = get_entry_from_table(key, scope);
      if(queryEntry != NULL){
        printf("Já declarado");
        exit(ERR_DECLARED);
      }

      return false;
}