#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erros_etapa4.h"
#include "natureza.h"
#include "auxiliary.h"

void insert_var_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador) {
	if (!curr_scope) {
		printf("exit-55\n");
		exit(-55);
	}

	char* identificador = (char*)(vl_identificador->value.string);
	if (key_exist(identificador, curr_scope)) {
		// re-declaracao de variavel no mesmo escopo:
		printf("ERR_DECLARED linha %d\n", vl_identificador->line);
		exit(ERR_DECLARED);
	}
	HASHTABLE_VALUE *new_hashtable_val = (HASHTABLE_VALUE*)malloc(sizeof(HASHTABLE_VALUE));
	new_hashtable_val->linha = vl_identificador->line;
	new_hashtable_val->natureza = NATUREZA_IDENTIFICADOR;

	char *tipo_escrito = vl_tipo->value.string;
	if (strcmp(tipo_escrito, "int") == 0) {
		new_hashtable_val->tipo = TIPO_INT;
		new_hashtable_val->tamanho = 4;
	}
	else if (strcmp(tipo_escrito, "float") == 0) {
		new_hashtable_val->tipo = TIPO_FLOAT;		
		new_hashtable_val->tamanho = 8;
	}
	else if (strcmp(tipo_escrito, "char") == 0) {
		new_hashtable_val->tipo = TIPO_CHAR;
		new_hashtable_val->tamanho = 1;		
	}
	else if (strcmp(tipo_escrito, "bool") == 0) {
		new_hashtable_val->tipo = TIPO_BOOL;
		new_hashtable_val->tamanho = 1;
	}
	else if (strcmp(tipo_escrito, "string") == 0) {
		new_hashtable_val->tipo = TIPO_STRING;		
		new_hashtable_val->tamanho = -1;  // nao tem como saber, pois 1 byte para cada caracter.
	}

	new_hashtable_val->func_arg_list = NULL;

	int ident_len = strlen(identificador);
	new_hashtable_val->key = (char*)malloc(sizeof(ident_len+1));
	strcpy(new_hashtable_val->key, identificador);

	new_hashtable_val->prox = NULL;

	// -----------------------------------------------------------------
	// insere na tabela hash:
	insert_in_hashtable(identificador, new_hashtable_val, curr_scope);
}

void insert_vec_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador) {

}

void insert_func_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, valor_lexico_list *param_list) {

}
