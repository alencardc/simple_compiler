/*
  Função principal para realização da E3.
  Não modifique este arquivo.
*/
#include <stdio.h>
#include "utils/iloc/instruction.h"
#include "utils/asm/asm.h"
extern int yyparse(void);
extern int yylex_destroy(void);

Instruction* iloc_code;

void *arvore = NULL;
void exporta (void *arvore);
void libera (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  //exporta (arvore);
  print_init_asm_code();
  generate_asm_code(iloc_code);
  print_final_asm_code();
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}