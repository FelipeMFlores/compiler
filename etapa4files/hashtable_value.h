#pragma once

// tipos para hashtable_value.
// tambem tipos inferidos do node
enum {
	// daqui ate TIPO_STRING, tambem e usado por FUNC_ARG_LIST.
	TIPO_INT = 30,	
	TIPO_FLOAT,
	TIPO_CHAR,
	TIPO_BOOL,
	TIPO_STRING,
	// ----------

	TIPO_INT_VEC,
	TIPO_FLOAT_VEC,
	TIPO_CHAR_VEC,
	TIPO_BOOL_VEC,
	TIPO_STRING_VEC,

	TIPO_INT_FUNC,
	TIPO_FLOAT_FUNC,
	TIPO_CHAR_FUNC,
	TIPO_BOOL_FUNC,
	TIPO_STRING_FUNC,



	TIPO_INVALIDO,

};

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

