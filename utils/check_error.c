#include "../errors.h"
#include "check_error.h"

bool check_identifier_redeclared(Table_Stack* scopes, char* key, int line){
  Symbol_Entry** scope = top_scope(scopes);

  Symbol_Entry* queryEntry = get_entry_from_table(key, scope);
  if(queryEntry != NULL){
    printf("[ERR_DECLARED] Identifier '%s' redeclared at line %d was previously "
      "declared at line %d.\n", key, line, queryEntry->line_number);
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

  printf("cast_type: %i", cast_type);
  printf("test_type: %i", test_type);

  printf("Tipo errado");
  exit(ERR_WRONG_TYPE);
}

bool check_identifier_undeclared(Table_Stack* scopes, char* key, int line) {
  Symbol_Entry* queryEntry = search_all_scopes(scopes, key);
  if(queryEntry == NULL) {
    printf("[ERR_UNDECLARED] Identifier '%s' at line %i was used without a previous declaration.\n", key, line);
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
    case VAR: printf("[ERR_VARIABLE] Variable '%s' was used as a vector at line %i.\n", entry->key, line);
      exit(ERR_VARIABLE);
      break;
    case FUNCTION: printf("[ERR_FUNCTION] Function '%s' was used as a vector at line %i.\n", entry->key, line);
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
    case VECTOR:  printf("[ERR_VECTOR] Vector '%s' was used as a variable at line %i.\n", entry->key, line);
      exit(ERR_VECTOR);
      break;
    case FUNCTION:  printf("[ERR_FUNCTION] Function '%s' was used as a variable at line %i.\n", entry->key, line);
      exit(ERR_FUNCTION);
    default:
      break;
  }
}

bool check_char_to_x(TokenValueType type1, TokenValueType type2, int line) {
  if (type1 == CHAR_VAL && type2 != CHAR_VAL
    || type2 == CHAR_VAL && type1 != CHAR_VAL) {
      TokenValueType coercion_type = type1 == CHAR_VAL ? type2 : type1;
      const char* type_string = get_type_name(coercion_type);
      printf("[ERR_CHAR_TO_X] Tried to do an implicit coercion of type 'char' to "
        "type %s at line %d.\n", type_string, line);

      exit(ERR_CHAR_TO_X);
  }

  return false;
}

bool check_string_to_x(TokenValueType type1, TokenValueType type2, int line) {
  if (type1 == STRING_VAL && type2 != STRING_VAL
    || type2 == STRING_VAL && type1 != STRING_VAL) {
      TokenValueType coercion_type = type1 == STRING_VAL ? type2 : type1;
      const char* type_string = get_type_name(coercion_type);
      printf("[ERR_STRING_TO_X] Tried to do an implicit coercion of type 'string' "
        "to type %s at line %d.\n", type_string, line);

      exit(ERR_STRING_TO_X);
  }

  return false;
}

bool check_invalid_coercion(TokenValueType type1, TokenValueType type2, int line) {
  check_char_to_x(type1, type2, line);
  check_string_to_x(type1, type2, line);
  return false;
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

bool check_function_string_par(TokenValueType type, int line) {
  if (type != STRING_VAL) {
    return false;
  }

  printf("[ERR_FUNCTION_STRING] Parameter declared at line %d is of type 'string'. "
    "Function's parameters can not be 'string'.\n", line);
  exit(ERR_FUNCTION_STRING);
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
    if(args->children_amount > 0)
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
    printf("[ERR_EXCESS_ARGS] Function '%s' expecting %i argument(s) but was called at line %i with %i argument(s).\n", 
            key, 
            declaration_arg_size, 
            actual_arg_size, 
            line);
    exit(ERR_EXCESS_ARGS);
  } else if(declaration_arg_size > actual_arg_size){
    printf("[ERR_MISSING_ARGS] Function '%s' expecting %i argument(s) but was called at line %i with only %i argument(s).\n", 
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

bool check_wrong_return_type(char* function_id, Table_Stack* scopes, TokenValueType type, int line){
  Symbol_Entry* function_entry = search_deep_scope(scopes,function_id);

  if(!is_type_compatible(function_entry->type, type)){
    char* function_type_str = get_type_name(function_entry->type);
    char* supplied_type_str = get_type_name(type);
    printf("[ERR_WRONG_PAR_RETURN] Returned type '%s' when function '\"%s\"' has return type '%s' at line %i.\n", supplied_type_str, function_id ,function_type_str, line);
    free(supplied_type_str);
    free(function_type_str);
    exit(ERR_WRONG_PAR_RETURN);
  }

  return false;
}

bool check_string_return_type(TokenValueType type, int line){
  if(type == STRING_VAL){
    printf("[ERR_FUNCTION_STRING] Returned type 'string' on line %i. Functions can't return 'string's.\n", line);
    exit(ERR_FUNCTION_STRING);
  }

  return false;
}

bool is_type_compatible(TokenValueType type1, TokenValueType type2){
  if(type1 == type2)
    return true;

  bool is_cast_type_valid = type1 == INTEGER_VAL || type1 == FLOAT_VAL ||  type1 == BOOL_VAL;
  bool is_test_type_valid = type2 == INTEGER_VAL || type2 == FLOAT_VAL || type2 == BOOL_VAL;
  
  return (is_cast_type_valid == true && is_test_type_valid == true);
  return false;
}

bool check_for_wrong_vector_string(Id_List* id, TokenValueType type, int line){
  if(id == NULL)
    return false;

  if(id->vector_size > 1 && type == STRING_VAL){
    printf("[ERR_STRING_VECTOR] Tried to declare \"%s\" as a string vector at line %i.\n", id->id ,line);
    exit(ERR_STRING_VECTOR);
  }

  return false;
}

bool check_for_assignment_type_error(Table_Stack* scopes, char* key, Node* value, int line){
  Symbol_Entry* entry = search_all_scopes(scopes, key);

  if(!is_type_compatible(entry->type, value->value_type)){
    char* variable_type = get_type_name(entry->type);
    char* value_type = get_type_name(value->value_type);
    printf("[ERR_WRONG_TYPE] Tried to assign a '%s' type to a variable of type '%s' at line %i.\n", value_type, variable_type, line);
    exit(ERR_WRONG_TYPE);
  }

  return false;
}

bool check_for_vector_assignment_type_error(Node* vector_node, Table_Stack* scopes, Node* value, int line){
  Node* id_node = vector_node->children[0];

  Symbol_Entry* vector_entry = search_all_scopes(scopes, id_node->label);

   if(!is_type_compatible(vector_entry->type, value->value_type)){
    char* variable_type = get_type_name(vector_entry->type);
    char* value_type = get_type_name(value->value_type);
    printf("[ERR_WRONG_TYPE] Tried to assign a '%s' type to a vector of type '%s' at line %i.\n", value_type, variable_type, line);
    exit(ERR_WRONG_TYPE);
  }

  return false;  
}

bool check_for_local_init_type_error(Symbol_Entry* entry, Node* value, int line){
  TokenValueType id_type = entry->type;
  TokenValueType value_type = value->value_type;


  if(!is_type_compatible(id_type, value_type)){
    char* id_type_str = get_type_name(id_type);
    char* value_type_str = get_type_name(value_type);
    printf("[ERR_WRONG_TYPE] Wrong initialization at line %i. Identifier '%s' expecting '%s' value, but '%s' was supplied.\n", line, entry->key, id_type_str, value_type_str);    
    exit(ERR_WRONG_TYPE);
  }

  return false;
}

bool check_error_string_max(Table_Stack* scopes, char* key, Node* value, int line){
  Symbol_Entry* var_entry = search_all_scopes(scopes, key);
  int entry_length = var_entry->length;


  if(var_entry->type == STRING_VAL && value->type == AST_LITERAL && value->value_type == STRING_VAL){
    int value_length = strlen(value->data->token_value.s_value);

    if(entry_length != value_length){
      printf("[ERR_STRING_MAX] Tried to assign a string of length %i to a string variable '%s' of length %i at line %i.\n", value_length, key, entry_length, line);
      exit(ERR_STRING_MAX);
    }
  } else if(var_entry->type == STRING_VAL && value->type == AST_IDENTIFIER){
    Symbol_Entry* var_value_entry = search_all_scopes(scopes, value->label);

    if(var_value_entry->type == STRING_VAL && var_value_entry->length != entry_length){
       printf("[ERR_STRING_MAX] Tried to assign a string variable of length %i to a string variable(\"%s\") of length %i, at line %i.\n", var_value_entry->length, key, entry_length, line);
       exit(ERR_STRING_MAX);
    }
  }

  return false;
}
