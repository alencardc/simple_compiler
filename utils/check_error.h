#ifndef __CHECK_ERROR_H__
#define __CHECK_ERROR_H__
#include "../utils/symbols_table/symbols_entry.h"

#include <stdbool.h>

bool check_identifier();

bool check_identifier_redeclared(Table_Stack* scopes, char* key);

#endif // __CHECK_ERROR_H__