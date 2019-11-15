/*
Função principal para realização da E3.

Este arquivo não pode ser modificado.
*/
#include <stdio.h>
#include "etapa5files/codeGenerator.h"
extern int yyparse(void);
extern int yylex_destroy(void);

void *arvore = NULL;
void libera (void *arvore);
void exporta (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  exporta (arvore);

  // gera codigo:
  generate_code(arvore);

  // codigo saida:
  //output_code(arvore);

  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}