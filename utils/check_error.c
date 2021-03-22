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

bool check_identifier_undeclared(Table_Stack* scopes, char* key){
     Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
      if(queryEntry == NULL){
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