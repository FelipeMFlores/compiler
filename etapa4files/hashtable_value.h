#pragma once

// Lista encadeada de argumentos de uma funcao.
typedef struct funcarglist {
	int tipo;  // tipo do argumento.
	struct funcarglist *prox;
} FUNC_ARG_LIST;


typedef struct hashtablevalue {
	int linha;  // localizacao da linha.
	int natureza;  // natureza (literal, variavel, funcao, etc.)
	int tipo;  // qual o tipo de dado deste simbolo.
	int tamanho;  // tamanho derivado do tipo.
	FUNC_ARG_LIST *func_arg_list;

	char *key;  // string do simbolo associado.

	struct hashtablevalue *prox;  // lista encadeada.

} HASHTABLE_VALUE;

