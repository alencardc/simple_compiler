#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"
#include "../ast/lex_value.h"

void test_string(char *string);

int main(){
    printf("------Test starting-------");
    char string1[] = "\"teste\"";
    char string2[] = "\'t\'";
    char string3[] = "\"linguica de feijao queimado e não muda nada e .com e .br\"";
    char string4[] = "\"\"\"";

    test_string(string1);
    test_string(string2);
    test_string(string3);
    test_string(string4);
}

void test_string(char *string){
    printf("\nString before: %s", string);
    remove_quotes(string);
    printf("\nString After: %s\n\n", string);
}