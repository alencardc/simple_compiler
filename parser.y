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

%define parse.error verbose

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
global_decl_list: storage_modifier type global_var_list ';' { printf("Global var\n");};

global_var_list: global_var_id
                 | global_var_list ',' global_var_id
                 ;

global_var_id: TK_IDENTIFICADOR
             | TK_IDENTIFICADOR '[' TK_LIT_INT ']'
             ;


/*************************************
**** Local variables declaration ****
*************************************/

local_decl: storage_modifier var_qualifier type local_var_list { printf("Local var decl\n");};

local_var_list: local_var_init
              | local_var_list ',' local_var_init
              ;

local_var_init: local_var_id
              | local_var_id TK_OC_LE local_var_value 
              ;

local_var_id: TK_IDENTIFICADOR;
local_var_value: TK_IDENTIFICADOR | literal ;


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

assign_expression: ternary_expression;

ternary_expression: or_expression
                  | or_expression '?' assign_expression ':' ternary_expression { printf("Ternary expression \n");}
                  ;

or_expression: and_expression
             | or_expression or_operator and_expression { printf("OR expression \n");}
             ;

or_operator: TK_OC_OR;

and_expression: bit_or_expression
              | and_expression and_operator bit_or_expression { printf("AND expression \n");}
              ;

and_operator: TK_OC_AND;

bit_or_expression: bit_and_expression
                 | bit_or_expression bit_or_operator bit_and_expression { printf("Bitwise_or expression \n");}
                 ;

bit_or_operator: '|';

bit_and_expression: equality_expression 
                  | bit_and_expression bit_and_operator equality_expression { printf("Bitwise_and expression \n");}
                  ;

bit_and_operator: '&';

equality_expression: relational_expression
                   | equality_expression equality_operator relational_expression { printf("Equational expression \n");}
                   ;

equality_operator: TK_OC_EQ
                 | TK_OC_NE
                 ;

relational_expression: additive_expression
                     | relational_expression relational_operator additive_expression { printf("Relational expression\n"); } 
                     ;

relational_operator: '<'
                   | '>'
                   | TK_OC_GE
                   | TK_OC_LE
                   ;

// Expressoes de shift???

additive_expression: multiplicative_expression
                  | additive_expression additive_operator multiplicative_expression { printf("Add expression\n"); }
                  ;

additive_operator: '+' | '-';

multiplicative_expression: exponential_expression
                         | multiplicative_expression multiplicative_operator exponential_expression { printf("Mult expression\n"); }
                         ;

multiplicative_operator: '*'
                       | '/'
                       | '%'
                       ;

exponential_expression: unary_expression
                      | exponential_expression exponential_operator unary_expression {printf("Exponential expression\n");}
                      ;

exponential_operator: '^';

unary_expression: basic_expression { printf("Basic expression\n"); }
                | unary_operator unary_expression { printf("Unary expression\n"); }
                ;

unary_operator: '+'
              | '-'
              | '!'
              | '&'
              | '*'
              | '?'
              | '#'
              ;

basic_expression: local_var_id { printf("[var id]\n"); }
                | TK_IDENTIFICADOR '['assign_expression ']' {printf("Vetorzao brabu\n");}
                | constant { printf("[constant]\n"); }
                | function_call
                | '(' assign_expression ')' { printf("[()]\n"); }
                ;

constant: TK_LIT_INT | TK_LIT_FLOAT ;

/*************************************
************* Commands ***************
*************************************/

command_list: generic_command
            | command_list generic_command
            ;

generic_command: one_line_command | multiline_command;

one_line_command: command ';';
command: local_decl
       | block_command
       | assign_command
       | io_command
       | shift_command
       | function_call
       | control_commands
       ;

control_block: '{' command_list '}' | '{' '}';
block_command: control_block;

assign_command: TK_IDENTIFICADOR '=' assign_expression 
              | TK_IDENTIFICADOR '[' assign_expression ']' '=' assign_expression  
              ;

input_command: TK_PR_INPUT TK_IDENTIFICADOR;
output_command: TK_PR_OUTPUT TK_IDENTIFICADOR 
              | TK_PR_OUTPUT literal 
              ;
io_command: input_command | output_command;

shift: TK_OC_SR | TK_OC_SL;
shift_operand: TK_IDENTIFICADOR | TK_IDENTIFICADOR '[' assign_expression ']'; 
shift_command: shift_operand shift TK_LIT_INT{printf("shiftzinho\n");};

function_call: TK_IDENTIFICADOR '(' arguments ')'{printf("chamou função\n");};

/*Podemos ter uma lista de 1 ou + argumentos ou nenhum*/
arguments: arguments_list | %empty;
arguments_list: argument | argument ',' arguments_list;
argument: assign_expression; 

control_commands: return | TK_PR_BREAK | TK_PR_CONTINUE;
return: TK_PR_RETURN assign_expression; 


/*************************************
********* Controle de fluxo **********
*************************************/
multiline_command: if_simples | if_else | for | while;

if_simples: TK_PR_IF '('assign_expression ')' control_block {printf("Um if\n");}; 
if_else: if_simples TK_PR_ELSE control_block {printf("if e else né\n");};

for: TK_PR_FOR '(' assign_command ':' assign_expression ':' assign_command ')' control_block {printf("Pegou um for\n");};  
while: TK_PR_WHILE '(' assign_expression ')' TK_PR_DO control_block {printf("Pegou um while\n");};; 

%%

void yyerror (char const *s) {
  printf("Error: %s at line %d\n", s, get_line_number());
}
