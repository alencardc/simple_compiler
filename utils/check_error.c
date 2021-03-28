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

bool check_identifier_undeclared(Table_Stack* scopes, char* key, int line) {
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry == NULL) {
    printf("[ERR_UNDECLARED] Identifier \"%s\" used without previous declaration at line %i.\n", key, line);
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

bool check_wrong_par_output(const char* key, LexValue literal, Table_Stack* scopes, int line){
  TokenValueType type;
  if (key == NULL){
    type = literal.value_type;
  }
  else{
    Symbol_Entry* entry = search_all_scopes(scopes,key);
    if(entry == NULL)
      return true;

    type = entry->type;
  }
  
  if(type != FLOAT_VAL && type != INTEGER_VAL){
    char* type_name = get_type_name(type);
    printf("[ERR_WRONG_PAR_OUTPUT] Output command expects (float/integer) value but type (%s) was supplied at line %i.\n", get_type_name(type), line);
    free(type_name);
    exit(ERR_WRONG_PAR_OUTPUT);
  }
}

char* get_type_name(TokenValueType type){
  char* type_name = (char*) malloc(sizeof(char) * 15);
  switch (type)
  {
  case FLOAT_VAL: type_name = strdup("float");
    break;
  case INTEGER_VAL: type_name = strdup("int");
    break;
  case BOOL_VAL: type_name = strdup("bool");
    break;
  case STRING_VAL: type_name = strdup("string");
    break;
  case CHAR_VAL: type_name = strdup("char");
    break;
  default:
    break;
  }

  return type_name;
}

bool check_wrong_arg_type(Node *args, const char* key, Table_Stack* scopes, int line){
  Symbol_Entry* entry = search_all_scopes(scopes, key);
  Argument_List* arg_list = entry->arg_list;
  int i = 1;
  while(arg_list != NULL){
    TokenValueType agr_type = arg_list->type;
    TokenValueType supplied_type = args->value_type;

    char* arg_type_str = get_type_name(agr_type);
    char* supplied_type_str = get_type_name(supplied_type);

    if(agr_type != supplied_type){
      printf("[ERR_WRONG_TYPE_ARGS] The %i° argument of function call [%s()] is wrong at line %i. Expected: %s | Actual: %s\n",
      i,
      entry->key,
      line,
      arg_type_str,
      supplied_type_str
      );
      exit(ERR_WRONG_TYPE_ARGS);
    }
    free(arg_type_str);
    free(supplied_type_str);

    i++;
    args = args->children[args->children_amount - 1];
    arg_list = arg_list->next;
  }
  return false;
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

bool check_wrong_par_shift(LexValue value){
  if(value.token_value.i_val > 16){
    printf("[ERR_WRONG_PAR_SHIFT] Shift command used a value (%i) greater than 16 at line %i.\n", value.token_value.i_val, value.line_number);
    exit(ERR_WRONG_PAR_SHIFT);
  }
  return false;
}