/*
Função principal para realização da E3.

Este arquivo não pode ser modificado.
*/
#include <stdio.h>
#include "tree.h"
extern int yyparse(void);
extern int yylex_destroy(void);


void *arvore = NULL;
void libera (void *arvore);
void exporta (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse(); 

  printTree(arvore);

  exporta (arvore);
  libera(arvore);
  arvore = NULL;
  yylex_destroy();
  return ret;
}