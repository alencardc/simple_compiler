#ifndef __CHECK_ERROR_H__
#define __CHECK_ERROR_H__
#include "../utils/symbols_table/symbols_entry.h"

#include <stdbool.h>

bool check_identifier_redeclared(Table_Stack* scopes, char* key);

bool check_type_cast(Node* node_cast, Node* node_test);
bool check_identifier_undeclared(Table_Stack* scopes, char* key);

bool check_wrong_vector(Table_Stack* scopes, char* key);
void errors_as_vector(Symbol_Entry* entry);

bool check_wrong_var(Table_Stack* scopes, char* key);
void errors_as_var(Symbol_Entry* entry);

#endif // __CHECK_ERROR_H__