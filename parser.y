%{
#include <stdio.h>
#include "utils/ast/ast.h"

int yylex(void);
void yyerror (char const *s);
extern int get_line_number(void);

extern void *arvore;

%}

%define parse.error verbose

%union {
  LexValue lexical_value;
  Node* node;
}

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
%token <lexical_value> TK_OC_LE
%token <lexical_value> TK_OC_GE
%token <lexical_value> TK_OC_EQ
%token <lexical_value> TK_OC_NE
%token <lexical_value> TK_OC_AND
%token <lexical_value> TK_OC_OR

%token <lexical_value> TK_OC_SL
%token <lexical_value> TK_OC_SR
%token <lexical_value> TK_LIT_INT
%token <lexical_value> TK_LIT_FLOAT
%token <lexical_value> TK_LIT_FALSE
%token <lexical_value> TK_LIT_TRUE
%token <lexical_value> TK_LIT_CHAR
%token <lexical_value> TK_LIT_STRING
%token <lexical_value> TK_IDENTIFICADOR
%token TOKEN_ERRO

%type <node> programa

%type <node> identifier
%type <node> vector_identifier
%type <node> literal
%type <node> local_var_init
%type <node> local_var_list
%type <node> local_decl
%type <node> local_var_value
%type <node> local_var_operator

%type <node> assign_expression;
%type <node> ternary_expression
%type <node> or_expression
%type <node> or_operator
%type <node> and_expression
%type <node> and_operator
%type <node> bit_or_expression
%type <node> bit_or_operator
%type <node> bit_and_expression
%type <node> bit_and_operator
%type <node> equality_expression
%type <node> equality_operator
%type <node> relational_expression
%type <node> relational_operator
%type <node> additive_expression
%type <node> additive_operator
%type <node> multiplicative_expression
%type <node> multiplicative_operator
%type <node> exponential_expression
%type <node> exponential_operator
%type <node> unary_operator
%type <node> unary_expression
%type <node> basic_expression
%type <node> constant 

%type <node> output_command
%type <node> input_command
%type <node> io_command
%type <node> command
%type <node> command_list
%type <node> one_line_command
%type <node> generic_command
%type <node> multiline_command
%type <node> block_command
%type <node> assign_command
%type <node> control_commands

%type <lexical_value> shift
%type <node> shift_operand
%type <node> shift_command
%type <node> shift_number

%type <node> func_decl
%type <node> function_call
%type <node> arguments
%type <node> arguments_list
%type <node> argument

%type <node> while
%type <node> control_block
%type <node> if_simples
%type <node> if_else
%type <node> for

%type <node> return

%start root

%%

root: programa { arvore = (void*)$1; }

programa: global_decl_list programa {}
        | func_decl programa { $$ = $1; append_child($$, $2); }
        | %empty { $$ = NULL; }
        ;

storage_modifier: TK_PR_STATIC | %empty;

var_qualifier: TK_PR_CONST | %empty;

identifier: TK_IDENTIFICADOR { $$ = create_node_with_lex($1); };

vector_identifier: identifier '[' assign_expression ']' { $$ = create_id_vector_node($1,$3); };

type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING;

literal: TK_LIT_INT { $$ = create_node_with_lex($1); }
       | TK_LIT_FLOAT { $$ = create_node_with_lex($1); }
       | TK_LIT_FALSE { $$ = create_node_with_lex($1); }
       | TK_LIT_TRUE { $$ = create_node_with_lex($1); }
       | TK_LIT_CHAR { $$ = create_node_with_lex($1); }
       | TK_LIT_STRING { $$ = create_node_with_lex($1); }
       ;


/*************************************
**** Global variables declaration ****
*************************************/
global_decl_list: storage_modifier type global_var_list ';';

global_var_list: global_var_id
                 | global_var_list ',' global_var_id
                 ;

global_var_id: identifier
             | identifier '[' vector_length ']'
             ;

vector_length: TK_LIT_INT | '+' TK_LIT_INT;


/*************************************
**** Local variables declaration ****
*************************************/

local_decl: storage_modifier var_qualifier type local_var_list { $$ = $4; };

local_var_list: local_var_init { $$ = $1; }
              | local_var_init ',' local_var_list { $$ = $1; append_child($$, $3); }
              ;

local_var_init: identifier { $$ = NULL; free_node($1); }
              | identifier local_var_operator local_var_value { $$ = create_binary_exp($2, $1, $3); }
              ;

local_var_operator: TK_OC_LE { $$ = create_node_with_lex($1); }

//local_var_id: TK_IDENTIFICADOR;
local_var_value: identifier | vector_identifier | literal ;


/*************************************
******* Functions declaration ********
*************************************/

func_decl: storage_modifier type identifier '(' params ')' control_block { $$ = $3; append_child($$, $7); };

params: param_list | %empty;

param_list: param
          | param_list ',' param
          ;

param: var_qualifier type identifier { free_node($3); };


/*************************************
*********** Expressions **************
*************************************/

assign_expression: ternary_expression { $$ = $1; };

ternary_expression: or_expression { $$ = $1; }
                  | or_expression '?' assign_expression ':' ternary_expression { $$ = create_ternary_tree("?:", $1, $3, $5); }
                  ;

or_expression: and_expression { $$ = $1; }
             | or_expression or_operator and_expression { $$ = create_binary_exp($2, $1, $3); }
             ;

or_operator: TK_OC_OR { $$ = create_node_with_lex($1); };

and_expression: bit_or_expression { $$ = $1; }
              | and_expression and_operator bit_or_expression { $$ = create_binary_exp($2, $1, $3); }
              ;

and_operator: TK_OC_AND { $$ = create_node_with_lex($1); };

bit_or_expression: bit_and_expression { $$ = $1; }
                 | bit_or_expression bit_or_operator bit_and_expression { $$ = create_binary_exp($2, $1, $3); }
                 ;

bit_or_operator: '|' { $$ = create_node_with_label("|"); };

bit_and_expression: equality_expression { $$ = $1; }
                  | bit_and_expression bit_and_operator equality_expression { $$ = create_binary_exp($2, $1, $3); }
                  ;

bit_and_operator: '&' { $$ = create_node_with_label("&"); };

equality_expression: relational_expression { $$ = $1; }
                   | equality_expression equality_operator relational_expression { $$ = create_binary_exp($2, $1, $3); }
                   ;

equality_operator: TK_OC_EQ { $$ = create_node_with_lex($1); }
                 | TK_OC_NE { $$ = create_node_with_lex($1); }
                 ;

relational_expression: additive_expression { $$ = $1; }
                     | relational_expression relational_operator additive_expression { $$ = create_binary_exp($2, $1, $3); }
                     ;

relational_operator: '<' { $$ = create_node_with_label("<"); }
                   | '>' { $$ = create_node_with_label(">"); }
                   | TK_OC_GE { $$ = create_node_with_lex($1); }
                   | TK_OC_LE { $$ = create_node_with_lex($1); }
                   ;

// Expressoes de shift???

additive_expression: multiplicative_expression { $$ = $1; }
                  | additive_expression additive_operator multiplicative_expression { $$ = create_binary_exp($2, $1, $3); }
                  ;

additive_operator: '+' { $$ = create_node_with_label("+"); }
                 | '-' { $$ = create_node_with_label("-"); }
                 ;

multiplicative_expression: exponential_expression { $$ = $1; }
                         | multiplicative_expression multiplicative_operator exponential_expression { $$ = create_binary_exp($2, $1, $3); }
                         ;

multiplicative_operator: '*' { $$ = create_node_with_label("*"); }
                       | '/' { $$ = create_node_with_label("/"); }
                       | '%' { $$ = create_node_with_label("%"); }
                       ;

exponential_expression: unary_expression { $$ = $1; }
                      | exponential_expression exponential_operator unary_expression { $$ = create_binary_exp($2, $1, $3); }
                      ;

exponential_operator: '^' { $$ = create_node_with_label("^"); } ;

unary_expression: basic_expression { $$ = $1; }
                | unary_operator unary_expression { $$ = $1; append_child($$, $2); }
                ;

unary_operator: '+' { $$ = create_node_with_label("+"); }
              | '-' { $$ = create_node_with_label("-"); }
              | '!' { $$ = create_node_with_label("!"); }
              | '&' { $$ = create_node_with_label("&"); }
              | '*' { $$ = create_node_with_label("*"); }
              | '?' { $$ = create_node_with_label("?"); }
              | '#' { $$ = create_node_with_label("#"); }
              ;

basic_expression: identifier { $$ = $1; }
                | vector_identifier { $$ = $1; }
                | constant { $$ = $1; }
                | function_call { $$ = $1; }
                | '(' assign_expression ')' { $$ = $2 ; }
                ;

constant: TK_LIT_INT { $$ = create_node_with_lex($1); }
        | TK_LIT_FLOAT { $$ = create_node_with_lex($1); }
        ;

/*************************************
************* Commands ***************
*************************************/

command_list: generic_command
              {$$ = $1;}
            |  generic_command command_list
                                          { 
                                            append_child($1,$2);
                                            $$ = $1; 
                                          };

generic_command: one_line_command | multiline_command {$$ = $1;};

one_line_command: command ';' {$$ = $1;};
command: local_decl
       | block_command
       | assign_command
       | io_command
       | shift_command
       | function_call { $$ = $1; }
       | control_commands
       {
         $$ = $1;
       }
       ;

control_block: '{' command_list '}'  
{
  Node* current_command = $2;
  //exporta((void*)current_command);
  $$ = $2;
  // int i = 1;
  // printf("Comando %i: %s\n", i, current_command->label);
  // current_command = current_command->children->next;
  // i++;
  // while(current_command != NULL){
  //   printf("Comando %i: %s\n", i,current_command->label);
  //   current_command = current_command->next; 
  //   i++;
  // }
};
block_command: control_block {};

assign_command: identifier '=' assign_expression { $$ = create_binary_tree("=", $1, $3); }
              | vector_identifier '=' assign_expression { $$ = create_binary_tree("=", $1, $3); }
              ;

input_command: TK_PR_INPUT identifier { $$ = create_io_node($2, "input"); };
output_command: TK_PR_OUTPUT identifier { $$ = create_io_node($2, "output"); }
               |  TK_PR_OUTPUT literal { $$ = create_io_node($2, "output"); }
              ;
              
io_command: input_command | output_command { $$ = $1; };

shift: TK_OC_SR | TK_OC_SL { $$ = $1;};
shift_operand: identifier { $$ = $1; } 
              | vector_identifier { $$ = $1; };

shift_command: shift_operand shift shift_number { $$ = create_shift_node($2, $1, $3); };
shift_number: TK_LIT_INT { $$ = create_node_with_lex($1); };
            | '+' TK_LIT_INT { $$ = create_node_with_lex($2); };

function_call: TK_IDENTIFICADOR '(' arguments ')' { $$ = create_func_call_node($1, $3); };

/*Podemos ter uma lista de 1 ou + argumentos ou nenhum*/
arguments: arguments_list {$$ = $1;} | %empty {$$ = NULL;};
arguments_list: argument {$$ = $1;} | argument ',' arguments_list {$$ = $1; append_child($$, $3);};
argument: assign_expression {$$ = $1;};


control_commands: return { $$ = $1; }
                | TK_PR_BREAK {$$ = create_node_with_label("break");}
                | TK_PR_CONTINUE {$$ = create_node_with_label("continue");}
                ;

return: TK_PR_RETURN assign_expression { $$ = create_node_with_label("return"); append_child($$, $2);}; 


/*************************************
********* Controle de fluxo **********
*************************************/
multiline_command: if_simples {$$ = $1;}
                 | if_else {$$ = $1;}
                 | for {$$ = $1;}
                 | while {$$ = $1;}
                 ;

if_simples: TK_PR_IF '(' assign_expression ')' control_block {$$ = create_partial_if_node($3,$5);}; 
if_else: if_simples TK_PR_ELSE control_block {append_child($1,$3); $$ = $1;};

for: TK_PR_FOR '(' assign_command ':' assign_expression ':' assign_command ')' control_block 
  {
    $$ = create_for_node($3,$5,$7,$9);
  };  
while: TK_PR_WHILE '(' assign_expression ')' TK_PR_DO control_block {$$ = create_while_node($3,$6);}; 

%%

void yyerror (char const *s) {
  printf("Error: %s at line %d\n", s, get_line_number());
}
