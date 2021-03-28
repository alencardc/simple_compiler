#ifndef __CHECK_ERROR_H__
#define __CHECK_ERROR_H__
#include "../utils/symbols_table/symbols_entry.h"

#include <stdbool.h>

bool check_identifier_redeclared(Table_Stack* scopes, char* key);

bool check_type_cast(Node* node_cast, Node* node_test);
bool check_identifier_undeclared(Table_Stack* scopes, char* key, int line);

bool check_wrong_vector(Table_Stack* scopes, char* key, int line);
void errors_as_vector(Symbol_Entry* entry, int line);

bool check_wrong_var(Table_Stack* scopes, char* key, int line);
void errors_as_var(Symbol_Entry* entry, int line);

bool check_wrong_par_input(int line, const char* key, Table_Stack* scopes);
bool check_wrong_par_output(const char* key, LexValue literal, Table_Stack* scopes, int line);
bool check_wrong_par_shift(LexValue value);

char* get_type_name(TokenValueType type);

bool check_wrong_arg_size(Node* args, const char* key, Table_Stack* scopes, int line);
bool check_wrong_arg_type(Node *args, const char* key, Table_Stack* scopes, int line);
#endif // __CHECK_ERROR_H__