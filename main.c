/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include "utils/optimizations/optimize_iloc.h"
#include "utils/iloc/instruction.h"
#include "utils/symbols_table/table_stack.h"
#include "utils/asm/asm.h"
extern int yyparse(void);
extern int yylex_destroy(void);

Table_Stack* scopes = NULL;
Instruction* iloc_code;

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);
void print_command_line_helper();

int main (int argc, char **argv)
{
  if(argc > 2){
    print_command_line_helper();
    return -1;
  }
  if(argc == 2 && strcmp(argv[1], "-O") != 0){
    print_command_line_helper();
    return -1;
  }
    
  int ret = yyparse(); 
  //exporta (arvore);
  printf("\n\n");

  if(argc == 2 && strcmp(argv[1], "-O") == 0){
    iloc_code = optimize_iloc_code(iloc_code);
  }

  generate_asm_code(iloc_code, scopes->table);

  pop_scope(scopes);

  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}

void print_command_line_helper(){
  printf("To compile with optimizations use the -O flag.\nUsage: ./etapa7 -O nomearquivo\n");
}