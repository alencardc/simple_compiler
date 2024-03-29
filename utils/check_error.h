#ifndef __CHECK_ERROR_H__
#define __CHECK_ERROR_H__
#include "../utils/symbols_table/symbols_entry.h"

#include <stdbool.h>

bool check_identifier_redeclared(Table_Stack* scopes, char* key, int line);

bool check_type_cast(Node* node_cast, Node* node_test);
bool check_identifier_undeclared(Table_Stack* scopes, char* key, int line);

bool check_wrong_vector(Table_Stack* scopes, char* key, int line);
bool check_for_wrong_vector_string(Id_List* id, TokenValueType type, int line);
void errors_as_vector(Symbol_Entry* entry, int line);

bool check_wrong_var(Table_Stack* scopes, char* key, int line);
void errors_as_var(Symbol_Entry* entry, int line);

bool check_wrong_function(Table_Stack* scopes, char* key, int line);
void errors_as_function(Symbol_Entry* entry, int line);

bool check_error_string_max(Table_Stack* scopes, char* key, Node* value, int line);
bool check_char_to_x(TokenValueType type1, TokenValueType type2, int line);
bool check_string_to_x(TokenValueType type1, TokenValueType type2, int line);
bool check_invalid_coercion(TokenValueType type1, TokenValueType type2, int line);

bool check_wrong_par_input(int line, const char* key, Table_Stack* scopes);
bool check_wrong_par_output(const char* key, LexValue literal, Table_Stack* scopes, int line);
bool check_wrong_par_shift(LexValue value);

char* get_type_name(TokenValueType type);

bool check_function_string_par(TokenValueType type, int line);
bool check_wrong_arg_size(Node* args, const char* key, Table_Stack* scopes, int line);
bool check_wrong_arg_type(Node *args, const char* key, Table_Stack* scopes, int line);
bool check_arg_redeclared(Table_Stack* scopes, char* key, int arg_number_err, int line);
bool check_wrong_return_type(char* function_id, Table_Stack* scopes, TokenValueType type, int line);
bool check_string_return_type(TokenValueType type, int line);
bool is_type_compatible(TokenValueType type1, TokenValueType type2);

bool check_for_assignment_type_error(Table_Stack* scopes, char* key, Node* value, int line);
bool check_for_vector_assignment_type_error(Node* vector_node, Table_Stack* scopes, Node* value, int line);

bool check_for_local_init_type_error(Symbol_Entry* entry, Node* value, int line);

bool check_identifier_exp(Table_Stack* scopes, Node* node);

#endif // __CHECK_ERROR_H__