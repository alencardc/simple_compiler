%{
#include <stdio.h>

/*
TODO
- Verificar as gramaticas que usam TK_IDENTIFICADOR, pois
essas podem ser que precisem aceitar a variação para vetor. 
Ou seja, além de um TK_IDENTIFICADOR, aceitar tamém
TK_IDENTIFICADOR '[' TK_LIT_INT ']'

trocar TK_LIT_INT como expressao no shift
trocar argument para aceitar expressões
*/

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
        | func_decl programa
        | %empty
        ;

storage_modifier: TK_PR_STATIC | %empty;
var_qualifier: TK_PR_CONST | %empty;
type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING;
literal: TK_LIT_INT | TK_LIT_FLOAT | TK_LIT_FALSE | TK_LIT_TRUE | TK_LIT_CHAR | TK_LIT_STRING;


/*************************************
**** Global variables declaration ****
*************************************/
global_decl_list: storage_modifier type global_var_list ';' { printf("Identifiquei\n");};

global_var_list: global_var_id
                 | global_var_list ',' global_var_id
                 ;

global_var_id: TK_IDENTIFICADOR
             | TK_IDENTIFICADOR '[' TK_LIT_INT ']'
             ;


/*************************************
**** Local variables declaration ****
*************************************/

local_decl: storage_modifier var_qualifier type local_var_list ';' { printf("Identifiquei\n");};

local_var_list: local_var_init
              | local_var_list ',' local_var_init
              ;

local_var_init: local_var_id
              | local_var_id TK_OC_LE assign_expression 
              ;
local_var_id: TK_IDENTIFICADOR;


/*************************************
******* Functions declaration ********
*************************************/

func_decl: storage_modifier type TK_IDENTIFICADOR '(' params ')' control_block;

params: param_list | %empty;

param_list: param
          | param_list ',' param
          ;

param: var_qualifier type TK_IDENTIFICADOR;


/*************************************
*********** Expressions **************
*************************************/

assign_expression: local_var_id | literal;


/*************************************
************* Commands ***************
*************************************/

command_list: command
            | command_list command
            ;

command: local_decl
       | block_command
       | assign_command
       | io_command
       | shift_command
       | function_call
       ;

control_block: '{' command_list '}' | '{' '}';
block_command: control_block ';' ;

assign_command: TK_IDENTIFICADOR '=' assign_expression ';'
              | TK_IDENTIFICADOR '[' TK_LIT_INT ']' '=' assign_expression ';'
              ;

input_command: TK_PR_INPUT TK_IDENTIFICADOR ';';
output_command: TK_PR_OUTPUT TK_IDENTIFICADOR ';'
              | TK_PR_OUTPUT literal ';'
              ;
io_command: input_command | output_command;

shift: TK_OC_SR | TK_OC_SL;
shift_operand: TK_IDENTIFICADOR | TK_IDENTIFICADOR '[' TK_LIT_INT ']'; /*Substituir por uma expressao*/
shift_command: shift_operand shift TK_LIT_INT ';' {printf("shiftzinho\n");};

function_call: TK_IDENTIFICADOR '(' arguments ')' ';' {printf("chamou função\n");};
/*Podemos ter uma lista de 1 ou + argumentos ou nenhum*/
arguments: arguments_list | %empty;
arguments_list: argument | argument ',' arguments_list;
argument: assign_expression; /*Substituir por uma expressao*/

%%

void yyerror (char const *s) {
  printf("Error: %s at line %d\n", s, get_line_number());
}
