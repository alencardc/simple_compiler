%{
#include <stdio.h>
#include "utils/ast/ast.h"

int yylex(void);
void yyerror (char const *s);
extern int get_line_number(void);
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
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR

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

%type <node> identifier
%type <node> vector_identifier
%type <node> literal

%type <node> assign_expression;
%type <node> ternary_expression
%type <node> or_expression
%type <node> and_expression
%type <node> bit_or_expression
%type <node> bit_and_expression
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
%type <node> local_decl
%type <node> block_command
%type <node> assign_command
%type <node> function_call
%type <node> control_commands

%type <lexical_value> shift;
%type <node> shift_operand;
%type <node> shift_command;
%type <node> shift_number;


%%

programa: global_decl_list programa
        | func_decl programa
        | %empty
        ;

storage_modifier: TK_PR_STATIC | %empty;

var_qualifier: TK_PR_CONST | %empty;

identifier: TK_IDENTIFICADOR { $$ = create_node_with_lex($1); };

vector_identifier: identifier '[' assign_expression ']' { $$ = create_id_vector_node($1,$3); };

type: TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING;

literal: TK_LIT_INT { $$ = create_node(&$1, "some int"); }
       | TK_LIT_FLOAT { $$ = create_node(&$1, "some float"); }
       | TK_LIT_FALSE { }
       | TK_LIT_TRUE { }
       | TK_LIT_CHAR { }
       | TK_LIT_STRING { }
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

local_decl: storage_modifier var_qualifier type local_var_list {};

local_var_list: local_var_init
              | local_var_list ',' local_var_init
              ;

local_var_init: local_var_id
              | local_var_id TK_OC_LE local_var_value 
              ;

local_var_id: TK_IDENTIFICADOR;
local_var_value: identifier | vector_identifier | literal ;


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

assign_expression: ternary_expression { $$ = $1; };

ternary_expression: or_expression { $$ = $1; }
                  | or_expression '?' assign_expression ':' ternary_expression
                  ;

or_expression: and_expression { $$ = $1; }
             | or_expression or_operator and_expression
             ;

or_operator: TK_OC_OR;

and_expression: bit_or_expression { $$ = $1; }
              | and_expression and_operator bit_or_expression 
              ;

and_operator: TK_OC_AND;

bit_or_expression: bit_and_expression { $$ = $1; }
                 | bit_or_expression bit_or_operator bit_and_expression
                 ;

bit_or_operator: '|';

bit_and_expression: equality_expression { $$ = $1; }
                  | bit_and_expression bit_and_operator equality_expression 
                  ;

bit_and_operator: '&';

equality_expression: relational_expression { $$ = $1; }
                   | equality_expression equality_operator relational_expression { $$ = create_binary_exp($2, $1, $3); }
                   ;

equality_operator: TK_OC_EQ { $$ = create_node_with_label("=="); }
                 | TK_OC_NE { $$ = create_node_with_label("!="); }
                 ;

relational_expression: additive_expression { $$ = $1; }
                     | relational_expression relational_operator additive_expression { $$ = create_binary_exp($2, $1, $3); }
                     ;

relational_operator: '<' { $$ = create_node_with_label("<"); }
                   | '>' { $$ = create_node_with_label(">"); }
                   | TK_OC_GE { $$ = create_node_with_label(">="); }
                   | TK_OC_LE { $$ = create_node_with_label("<="); }
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
                | function_call
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
            | command_list generic_command{$$ = append_child($1,$2);}
            ;

generic_command: one_line_command | multiline_command {$$ = $1;};

one_line_command: command ';' {$$ = $1;};
command: local_decl
       | block_command
       | assign_command
       | io_command
       | shift_command
       | function_call
       | control_commands
       {
         $$ = $1;
       }
       ;

control_block: '{' command_list '}'  
{
  Node* current_command = $2;
  //exporta((void*)current_command);
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

assign_command: identifier '=' assign_expression { $$ = create_binary_tree("=", $1, $3); exporta((void*)$$);  }
              | vector_identifier '=' assign_expression { $$ = create_binary_tree("=", $1, $3); }
              ;

input_command: TK_PR_INPUT identifier 
              {
                $$ = create_io_node($2, "input");
              };
output_command: TK_PR_OUTPUT identifier
              {
                $$ = create_io_node($2, "output");
                
              }
               |  TK_PR_OUTPUT literal
              {
                $$ = create_io_node($2, "output");
               }
              ;
              
io_command: input_command | output_command { $$ = $1; };

shift: TK_OC_SR | TK_OC_SL { $$ = $1;};
shift_operand: identifier { $$ = $1; } 
              | vector_identifier { $$ = $1; };

shift_command: shift_operand shift shift_number { $$ = create_shift_node($2,$1,$3); };
shift_number: TK_LIT_INT {
                char* integerInString = integerToString($1.token_value.i_val);
                $$ = create_node(&$1, integerInString);
            };
            | '+' TK_LIT_INT 
            {
                char* integerInString = integerToString($2.token_value.i_val);
                $$ = create_node(&$2, integerInString);
            };

function_call: TK_IDENTIFICADOR '(' arguments ')' {  };

/*Podemos ter uma lista de 1 ou + argumentos ou nenhum*/
arguments: arguments_list | %empty;
arguments_list: argument | argument ',' arguments_list;
argument: assign_expression;


control_commands: return {}
                | TK_PR_BREAK {$$ = create_node(NULL, "break");}
                | TK_PR_CONTINUE {$$ = create_node(NULL, "continue");}
                ;

return: TK_PR_RETURN assign_expression; 


/*************************************
********* Controle de fluxo **********
*************************************/
multiline_command: if_simples {}
                 | if_else {}
                 | for {}
                 | while {}
                 ;

if_simples: TK_PR_IF '('assign_expression ')' control_block; 
if_else: if_simples TK_PR_ELSE control_block;

for: TK_PR_FOR '(' assign_command ':' assign_expression ':' assign_command ')' control_block;  
while: TK_PR_WHILE '(' assign_expression ')' TK_PR_DO control_block; 

%%

void yyerror (char const *s) {
  printf("Error: %s at line %d\n", s, get_line_number());
}
