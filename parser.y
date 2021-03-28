%{
#include <stdio.h>
#include <stdlib.h>
#include "utils/check_error.h"
#include "utils/ast/ast.h"
#include "errors.h"

int yylex(void);
void yyerror (char const *s);
extern int get_line_number(void);

char* function_id = NULL;
Table_Stack* scopes = NULL;

extern void *arvore;

%}

%define parse.error verbose

%union {
  LexValue lexical_value;
  Node* node;
  Id_List* id_list;
  TokenValueType type;
  Argument_List* arg_list;
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
%type <node> func_header

%type <id_list> global_var_id
%type <id_list> global_var_list
%type <type> type
%type <lexical_value> vector_length

%type <arg_list> param
%type <arg_list> param_list
%type <arg_list> params

%start root

%%

root: programa { arvore = (void*)$1; print_table_stack(scopes); }

programa: global_decl_list programa {$$ = $2;};
        | func_decl programa { $$ = $1; append_child($$, $2); }
        | %empty { $$ = NULL; }
        ;

storage_modifier: TK_PR_STATIC | %empty;

var_qualifier: TK_PR_CONST | %empty;

identifier: TK_IDENTIFICADOR { $$ = create_id_node($1); };

vector_identifier: identifier '[' assign_expression ']' { 
  if(!check_identifier_undeclared(scopes, $1->label, $1->data->line_number) 
  && !check_wrong_vector(scopes, $1->label, $1->data->line_number)){
    $$ = create_vector_node($1,$3);
  }
  Symbol_Entry* entry = search_all_scopes(scopes, $1->label);
  inject_value_type($$, entry->type);
};

type: TK_PR_INT  { $$ = INTEGER_VAL; }
    | TK_PR_FLOAT  { $$ = FLOAT_VAL; }
    | TK_PR_BOOL  { $$ = BOOL_VAL; }
    | TK_PR_CHAR { $$ = CHAR_VAL; }
    | TK_PR_STRING { $$ = STRING_VAL; };

literal: TK_LIT_INT { $$ = create_node_with_lex($1, AST_LITERAL); }
       | TK_LIT_FLOAT { $$ = create_node_with_lex($1, AST_LITERAL); }
       | TK_LIT_FALSE { $$ = create_node_with_lex($1, AST_LITERAL); }
       | TK_LIT_TRUE { $$ = create_node_with_lex($1, AST_LITERAL); }
       | TK_LIT_CHAR { $$ = create_node_with_lex($1, AST_LITERAL); }
       | TK_LIT_STRING { $$ = create_node_with_lex($1, AST_LITERAL); }
       ;


/*************************************
**** Global variables declaration ****
*************************************/
global_decl_list: storage_modifier type global_var_list ';'{
    if(scopes == NULL){
      scopes = push_new_scope(scopes, "global");
    }

    Id_List* id_list = $3;
    Symbol_Entry** global_scope = top_scope(scopes);

    while(id_list != NULL){
      if(!check_identifier_redeclared(scopes,id_list->id)){
        check_for_wrong_vector_string(id_list, $2, get_line_number());
        Symbol_Entry* new_entry = create_id_entry(id_list, $2);
      
        insert_entry_at_table(new_entry, global_scope);
      }
        id_list = id_list->next;
    }
    free_id_list($3);
};

global_var_list: global_var_id { $$ = $1;}
                 | global_var_list ',' global_var_id 
                 {
                   $$ = append_id_list($1,$3);
                  }
                 ;

global_var_id: identifier {  
    $$ = create_id_list($1->label, 1, $1->data->line_number);
    free_node($1);
  }
  | identifier '[' vector_length ']' {   
    $$ = create_id_list($1->label, $3.token_value.i_val, $1->data->line_number);
    free_node($1);
  };

vector_length: TK_LIT_INT {$$ = $1;} | '+' TK_LIT_INT {$$ = $2;};


/*************************************
**** Local variables declaration ****
*************************************/

local_decl: storage_modifier var_qualifier type local_var_list {
  insert_local_entry_from_list($4, $3, scopes);
  $$ = free_all_id_nodes($4);
};

local_var_list: local_var_init { $$ = $1; }
              | local_var_init ',' local_var_list { $$ = create_local_node($1, $3); }
              ;

local_var_init: identifier { $$ = $1;  }
              | identifier local_var_operator local_var_value { $$ = create_binary_exp($2, $1, $3); }
              ;

local_var_operator: TK_OC_LE { $$ = create_node_with_lex($1, AST_ASSIGN); }

//local_var_id: TK_IDENTIFICADOR;
local_var_value: identifier {
  check_identifier_undeclared(scopes, $1->label, $1->data->line_number);
  check_wrong_var(scopes, $1->label, $1->data->line_number);
  $$ = $1;
} | vector_identifier | literal ;


/*************************************
******* Functions declaration ********
*************************************/

func_header: storage_modifier type identifier '(' params ')'{
   if(scopes == NULL){
      scopes = push_new_scope(scopes, "global");
  }

  Symbol_Entry* function = create_function_entry($3->label, $5, $2, $3->data->line_number);
  Symbol_Entry** current_scope = top_scope(scopes);
  int i = insert_entry_at_table(function, current_scope);

  function_id = strdup($3->label);
  $$ = $3;
}

func_decl: func_header control_block {
  $$ = create_function_node($1, $2);
  if(function_id != NULL){
    free(function_id);
  }
};

params: param_list { $$ = $1; }| %empty { $$ = NULL; };

param_list: param { $$ = $1;}
          | param_list ',' param {$$ = append_arg_list($1, $3);}
          ;

param: var_qualifier type identifier 
  { 
    Argument_List* arg_list_element = create_arg_list_element($3->label, $2);
    $$ = arg_list_element;
    free_node($3); 
  };


/*************************************
*********** Expressions **************
*************************************/

assign_expression: ternary_expression { $$ = $1; };

ternary_expression: or_expression { $$ = $1; }
                  | or_expression '?' assign_expression ':' ternary_expression { $$ = create_ternary_node($1, $3, $5); }
                  ;

or_expression: and_expression { $$ = $1; }
             | or_expression or_operator and_expression { $$ = create_binary_exp($2, $1, $3); }
             ;

or_operator: TK_OC_OR { $$ = create_node_with_lex($1, AST_BINARY_EXP); };

and_expression: bit_or_expression { $$ = $1; }
              | and_expression and_operator bit_or_expression { $$ = create_binary_exp($2, $1, $3); }
              ;

and_operator: TK_OC_AND { $$ = create_node_with_lex($1, AST_BINARY_EXP); };

bit_or_expression: bit_and_expression { $$ = $1; }
                 | bit_or_expression bit_or_operator bit_and_expression { $$ = create_binary_exp($2, $1, $3); }
                 ;

bit_or_operator: '|' { $$ = create_node_with_label("|", AST_BINARY_EXP); };

bit_and_expression: equality_expression { $$ = $1; }
                  | bit_and_expression bit_and_operator equality_expression { $$ = create_binary_exp($2, $1, $3); }
                  ;

bit_and_operator: '&' { $$ = create_node_with_label("&", AST_BINARY_EXP); };

equality_expression: relational_expression { $$ = $1; }
                   | equality_expression equality_operator relational_expression { $$ = create_binary_exp($2, $1, $3); }
                   ;

equality_operator: TK_OC_EQ { $$ = create_node_with_lex($1, AST_BINARY_EXP); }
                 | TK_OC_NE { $$ = create_node_with_lex($1, AST_BINARY_EXP); }
                 ;

relational_expression: additive_expression { $$ = $1; }
                     | relational_expression relational_operator additive_expression { $$ = create_binary_exp($2, $1, $3); }
                     ;

relational_operator: '<' { $$ = create_node_with_label("<", AST_BINARY_EXP); }
                   | '>' { $$ = create_node_with_label(">", AST_BINARY_EXP); }
                   | TK_OC_GE { $$ = create_node_with_lex($1, AST_BINARY_EXP); }
                   | TK_OC_LE { $$ = create_node_with_lex($1, AST_BINARY_EXP); }
                   ;

additive_expression: multiplicative_expression { $$ = $1; }
                  | additive_expression additive_operator multiplicative_expression { $$ = create_binary_exp($2, $1, $3); }
                  ;

additive_operator: '+' { $$ = create_node_with_label("+", AST_BINARY_EXP); }
                 | '-' { $$ = create_node_with_label("-", AST_BINARY_EXP); }
                 ;

multiplicative_expression: exponential_expression { $$ = $1; }
                         | multiplicative_expression multiplicative_operator exponential_expression { $$ = create_binary_exp($2, $1, $3);};
                                                                                                            

multiplicative_operator: '*' { $$ = create_node_with_label("*", AST_BINARY_EXP); }
                       | '/' { $$ = create_node_with_label("/", AST_BINARY_EXP); }
                       | '%' { $$ = create_node_with_label("%", AST_BINARY_EXP); }
                       ;

exponential_expression: unary_expression { $$ = $1; }
                      | exponential_expression exponential_operator unary_expression { $$ = create_binary_exp($2, $1, $3); }
                      ;

exponential_operator: '^' { $$ = create_node_with_label("^", AST_BINARY_EXP); } ;

unary_expression: basic_expression { $$ = $1; }
                | unary_operator unary_expression { $$ = $1;
                                                    $$->value_type =  $2->value_type;
                                                    append_child($$, $2);
                                                  }
                ;

unary_operator: '+' { $$ = create_node_with_label("+", AST_UNARY_EXP); }
              | '-' { $$ = create_node_with_label("-", AST_UNARY_EXP); }
              | '!' { $$ = create_node_with_label("!", AST_UNARY_EXP); }
              | '&' { $$ = create_node_with_label("&", AST_UNARY_EXP); }
              | '*' { $$ = create_node_with_label("*", AST_UNARY_EXP); }
              | '?' { $$ = create_node_with_label("?", AST_UNARY_EXP); }
              | '#' { $$ = create_node_with_label("#", AST_UNARY_EXP); }
              ;

basic_expression: 
  identifier {
    if(!check_identifier_undeclared(scopes, $1->label, $1->data->line_number)
      && !check_wrong_var(scopes, $1->label, $1->data->line_number)
    ){

      $$ = $1;
      inject_value_type_from_scopes($$, scopes);
    } 
  }
  | vector_identifier { $$ = $1;}
  | constant { $$ = $1; }
  | function_call { $$ = $1; }
  | '(' assign_expression ')' { $$ = $2 ; }
  ;

constant: TK_LIT_INT { $$ = create_node_with_lex($1, AST_LITERAL);
                       $$->value_type = INTEGER_VAL;
                     }
        | TK_LIT_FLOAT { $$ = create_node_with_lex($1, AST_LITERAL);
                         $$->value_type = FLOAT_VAL;
                       }
        | TK_LIT_TRUE { $$ = create_node_with_lex($1, AST_LITERAL); 
                        $$->value_type = BOOL_VAL;
                      }
        | TK_LIT_FALSE { $$ = create_node_with_lex($1, AST_LITERAL); 
                         $$->value_type = BOOL_VAL;
                       }
        ;

/*************************************
************* Commands ***************
*************************************/

command_list: generic_command { $$ = $1; }
            | generic_command command_list { $$ = $1; append_child($$,$2); }
            | local_decl ';' { $$ = $1; }
            | local_decl ';' command_list { $$ = join_local_with_commands($1, $3);}
            | block_command ';' { $$ = $1; }
            | block_command ';' command_list { $$ = join_command_lists($1, $3);}
            ;

generic_command: one_line_command {$$ = $1; }
               | multiline_command {$$ = $1;}
               ;

one_line_command: command ';' {$$ = $1; };

command: assign_command { $$ = $1; }
       | io_command { $$ = $1; }
       | shift_command { $$ = $1; }
       | function_call { $$ = $1; }
       | control_commands { $$ = $1; }
       ;

control_block: control_block_start command_list control_block_end { $$ = $2; };
block_command: control_block { $$ = $1; };
control_block_start: '{' { scopes = push_new_scope(scopes, ""); };
control_block_end: '}' { scopes = pop_scope(scopes); };

assign_command: identifier '=' assign_expression {  
                                                    check_identifier_undeclared(scopes, $1->label, $1->data->line_number);
                                                    check_wrong_var(scopes, $1->label, $1->data->line_number);
                                                    check_for_assignment_type_error(scopes, $1->label, $3, $1->data->line_number);
                                                    $$ = create_binary_tree("=", AST_ASSIGN,$1, $3); 
                                                  }
              | vector_identifier '=' assign_expression { 
                                                          check_for_vector_assignment_type_error($1, scopes, $3, get_line_number());            
                                                          $$ = create_binary_tree("=", AST_ASSIGN, $1, $3); 
                                                        }
              ;

input_command: TK_PR_INPUT identifier {
  $$ = create_io_node($2, "input"); 
  check_identifier_undeclared(scopes, $2->label, $2->data->line_number);
  check_wrong_par_input($2->data->line_number, $2->label, scopes);
  check_wrong_var(scopes, $2->label, $2->data->line_number);
};
output_command: TK_PR_OUTPUT identifier { 
                                          check_identifier_undeclared(scopes, $2->label, $2->data->line_number);
                                          check_wrong_var(scopes, $2->label, $2->data->line_number);
                                          check_wrong_par_output($2->label, *($2->data), scopes, $2->data->line_number);
                                          $$ = create_io_node($2, "output"); 
                                        }
              | TK_PR_OUTPUT literal { 
                                        check_wrong_par_output(NULL, *($2->data), scopes, $2->data->line_number);
                                        $$ = create_io_node($2, "output"); 
                                      }
              ;
              
io_command: input_command | output_command { $$ = $1; };

shift: TK_OC_SR | TK_OC_SL { $$ = $1;};
shift_operand: identifier {
                            check_identifier_undeclared(scopes, $1->label, $1->data->line_number);
                            check_wrong_var(scopes, $1->label, $1->data->line_number); 
                            $$ = $1; 
                          } 
              | vector_identifier { $$ = $1; };

shift_command: shift_operand shift shift_number { $$ = create_shift_node($2, $1, $3); };
shift_number: TK_LIT_INT {
                            check_wrong_par_shift($1); 
                            $$ = create_node_with_lex($1, AST_LITERAL); 
                          };
            | '+' TK_LIT_INT { 
                                check_wrong_par_shift($2);
                                $$ = create_node_with_lex($2, AST_LITERAL); 
                              };

function_call: TK_IDENTIFICADOR '(' arguments ')' {
  check_identifier_undeclared(scopes, $1.token_value.s_value, $1.line_number);
  check_wrong_arg_size($3, $1.token_value.s_value, scopes, $1.line_number);
  check_wrong_arg_type($3, $1.token_value.s_value, scopes, $1.line_number);
  $$ = create_func_call_node($1, $3);
  Symbol_Entry* entry = search_all_scopes(scopes, ($$->label + 5));
  $$->value_type = entry->type;
};

/*Podemos ter uma lista de 1 ou + argumentos ou nenhum*/
arguments: arguments_list {$$ = $1;} | %empty {$$ = NULL;};
arguments_list: argument {$$ = $1;} | argument ',' arguments_list {$$ = $1; append_child($$, $3);};
argument: assign_expression { $$ = $1; };



control_commands: return { $$ = $1; }
                | TK_PR_BREAK {$$ = create_node_with_label("break", AST_CONTROL);}
                | TK_PR_CONTINUE {$$ = create_node_with_label("continue", AST_CONTROL);}
                ;

return: TK_PR_RETURN assign_expression { 
                                          $$ = create_node_with_label("return", AST_RETURN); 
                                          append_child($$, $2);
                                          check_wrong_return_type(function_id, scopes, $2->value_type, get_line_number());

                                        }; 


/*************************************
********* Controle de fluxo **********
*************************************/
multiline_command: if_simples {$$ = $1;}
                 | if_else {$$ = $1;}
                 | for {$$ = $1;}
                 | while {$$ = $1;}
                 ;

if_simples: TK_PR_IF '(' assign_expression ')' control_block {$$ = create_if_node($3,$5);}; 
if_else: if_simples TK_PR_ELSE control_block { $$ = create_if_else_node($1, $3); };

for: TK_PR_FOR '(' assign_command ':' assign_expression ':' assign_command ')' control_block 
  {
    $$ = create_for_node($3,$5,$7,$9);
  };  
while: TK_PR_WHILE '(' assign_expression ')' TK_PR_DO control_block {$$ = create_while_node($3,$6);}; 

%%

void yyerror (char const *s) {
  printf("Error: %s at line %d\n", s, get_line_number());
}
