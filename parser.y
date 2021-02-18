%{
#include <stdio.h>

int yylex(void);
void yyerror (char const *s);
extern int get_line_number(void);
%}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO

%%

programa: global_decl_list programa
        | block programa
        |
        ;

var_storage_mod: TK_PR_STATIC | ;
var_qualifier: TK_PR_CONST | ;
type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING;
literal: TK_LIT_INT | TK_LIT_FLOAT | TK_LIT_FALSE | TK_LIT_TRUE | TK_LIT_CHAR | TK_LIT_STRING;

// Global variables declaration

global_decl_list: var_storage_mod type global_var_list ';' { printf("Identifiquei\n");};

global_var_list: global_var_id
                 | global_var_list ',' global_var_id
                 ;

global_var_id: TK_IDENTIFICADOR
             | TK_IDENTIFICADOR '[' TK_LIT_INT ']'
             ;



// Local variables declaration

local_decl_list: local_decl
               | local_decl_list local_decl
               ;

local_decl: var_storage_mod var_qualifier type local_var_list ';' { printf("Identifiquei\n");};

local_var_list: local_var_init
              | local_var_list ',' local_var_init
              ;

local_var_init: local_var_id
              | local_var_id TK_OC_LE assign_expression 
              ;
local_var_id: TK_IDENTIFICADOR;

// Expressions

assign_expression: local_var_id | literal;

block: '{' local_decl_list '}';

%%

void yyerror (char const *s) {
  printf("Error: %s at line %d\n", s, get_line_number());
}
