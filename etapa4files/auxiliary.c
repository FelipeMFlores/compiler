#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erros_etapa4.h"
#include "natureza.h"
#include "auxiliary.h"

int is_var(int tipo) {
	return (tipo == TIPO_INT || tipo == TIPO_FLOAT || tipo == TIPO_CHAR || tipo == TIPO_BOOL || tipo == TIPO_STRING);
}
int is_vec(int tipo) {
	return (tipo == TIPO_INT_VEC || tipo == TIPO_FLOAT_VEC || tipo == TIPO_CHAR_VEC || 
		tipo == TIPO_BOOL_VEC || tipo == TIPO_STRING_VEC);	
}
int is_func(int tipo) {
	return (tipo == TIPO_INT_FUNC || tipo == TIPO_FLOAT_FUNC || tipo == TIPO_CHAR_FUNC || 
		tipo == TIPO_BOOL_FUNC || tipo == TIPO_STRING_FUNC);		
}

// ------------------------------------------------------------------------------------------------------

void insert_var_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador) {
	if (!curr_scope) {
		exit(-55);
	}

	char* identificador = (char*)(vl_identificador->value.string);
	if (key_exist(identificador, curr_scope)) {
		// re-declaracao de variavel no mesmo escopo:
		printf("ERR_DECLARED variable linha %d\n", vl_identificador->line);
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

	new_hashtable_val->func_arg_list = NULL;  // nao eh funcao.

	int ident_len = strlen(identificador);
	new_hashtable_val->key = (char*)malloc(sizeof(char)*(ident_len+1));
	strcpy(new_hashtable_val->key, identificador);

	new_hashtable_val->prox = NULL;

	// -----------------------------------------------------------------
	// insere na tabela hash:
	insert_in_hashtable(identificador, new_hashtable_val, curr_scope);
}

void insert_vec_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador) {
	if (!curr_scope) {
		exit(-54);
	}

	char* identificador = (char*)(vl_identificador->value.string);
	if (key_exist(identificador, curr_scope)) {
		// re-declaracao de variavel ou vetor no mesmo escopo (global):
		printf("ERR_DECLARED vector linha %d\n", vl_identificador->line);
		exit(ERR_DECLARED);
	}
	HASHTABLE_VALUE *new_hashtable_val = (HASHTABLE_VALUE*)malloc(sizeof(HASHTABLE_VALUE));
	new_hashtable_val->linha = vl_identificador->line;
	new_hashtable_val->natureza = NATUREZA_IDENTIFICADOR;

	char *tipo_escrito = vl_tipo->value.string;
	if (strcmp(tipo_escrito, "int") == 0) {
		new_hashtable_val->tipo = TIPO_INT_VEC;
		new_hashtable_val->tamanho = 4;
	}
	else if (strcmp(tipo_escrito, "float") == 0) {
		new_hashtable_val->tipo = TIPO_FLOAT_VEC;		
		new_hashtable_val->tamanho = 8;
	}
	else if (strcmp(tipo_escrito, "char") == 0) {
		new_hashtable_val->tipo = TIPO_CHAR_VEC;
		new_hashtable_val->tamanho = 1;		
	}
	else if (strcmp(tipo_escrito, "bool") == 0) {
		new_hashtable_val->tipo = TIPO_BOOL_VEC;
		new_hashtable_val->tamanho = 1;
	}
	else if (strcmp(tipo_escrito, "string") == 0) {
		new_hashtable_val->tipo = TIPO_STRING_VEC;		
		new_hashtable_val->tamanho = -1;  // nao tem como saber, pois 1 byte para cada caracter.
	}

	new_hashtable_val->func_arg_list = NULL;  // nao eh funcao.
	
	int ident_len = strlen(identificador);
	new_hashtable_val->key = (char*)malloc(sizeof(char)*(ident_len+1));
	strcpy(new_hashtable_val->key, identificador);

	new_hashtable_val->prox = NULL;

	// -----------------------------------------------------------------
	// insere na tabela hash:
	insert_in_hashtable(identificador, new_hashtable_val, curr_scope);
	if (!curr_scope) {
		exit(-137);
	}
}

void insert_func_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, valor_lexico_list *param_list) {
	if (!curr_scope) {
		exit(-53);
	}

	char* identificador = (char*)(vl_identificador->value.string);
	if (key_exist(identificador, curr_scope)) {
		// re-declaracao de funcao no mesmo escopo:
		printf("ERR_DECLARED function linha %d\n", vl_identificador->line);
		exit(ERR_DECLARED);
	}

	HASHTABLE_VALUE *new_hashtable_val = (HASHTABLE_VALUE*)malloc(sizeof(HASHTABLE_VALUE));
	new_hashtable_val->linha = vl_identificador->line;
	new_hashtable_val->natureza = NATUREZA_IDENTIFICADOR;

	char *tipo_escrito = vl_tipo->value.string;
	if (strcmp(tipo_escrito, "int") == 0) {
		new_hashtable_val->tipo = TIPO_INT_FUNC;
		new_hashtable_val->tamanho = 4;
	}
	else if (strcmp(tipo_escrito, "float") == 0) {
		new_hashtable_val->tipo = TIPO_FLOAT_FUNC;		
		new_hashtable_val->tamanho = 8;
	}
	else if (strcmp(tipo_escrito, "char") == 0) {
		new_hashtable_val->tipo = TIPO_CHAR_FUNC;
		new_hashtable_val->tamanho = 1;		
	}
	else if (strcmp(tipo_escrito, "bool") == 0) {
		new_hashtable_val->tipo = TIPO_BOOL_FUNC;
		new_hashtable_val->tamanho = 1;
	}
	else if (strcmp(tipo_escrito, "string") == 0) {
		new_hashtable_val->tipo = TIPO_STRING_FUNC;		
		new_hashtable_val->tamanho = -1;  // nao tem como saber, pois 1 byte para cada caracter.
	}

	int ident_len = strlen(identificador);
	new_hashtable_val->key = (char*)malloc(sizeof(char)*(ident_len+1));
	strcpy(new_hashtable_val->key, identificador);
	new_hashtable_val->prox = NULL;	

	// -----------------------------------------------------------------
	// new_hashtable_val->func_arg_list:
	valor_lexico_list *param_percorre = param_list;
	valor_lexico *aux_params;
	FUNC_ARG_LIST *aux_construct;

	if (param_percorre == NULL) {  // funcao sem parametros.
		new_hashtable_val->func_arg_list = NULL;
	}
	else {
		new_hashtable_val->func_arg_list = (FUNC_ARG_LIST*)malloc(sizeof(FUNC_ARG_LIST));
		aux_construct = new_hashtable_val->func_arg_list;
		while (param_percorre != NULL) {
			aux_params = param_percorre->vl;

			// pega o tipo do argumento na list:
			tipo_escrito = aux_params->value.string;
			if (strcmp(tipo_escrito, "int") == 0) {
				aux_construct->tipo = TIPO_INT;
			}
			else if (strcmp(tipo_escrito, "float") == 0) {
				aux_construct->tipo = TIPO_FLOAT;
			}
			else if (strcmp(tipo_escrito, "char") == 0) {
				aux_construct->tipo = TIPO_CHAR;
			}
			else if (strcmp(tipo_escrito, "bool") == 0) {
				aux_construct->tipo = TIPO_BOOL;
			}
			else if (strcmp(tipo_escrito, "string") == 0) {
				aux_construct->tipo = TIPO_STRING;
			}
			//

			if (param_percorre->prox != NULL) {
				aux_construct->prox = (FUNC_ARG_LIST*)malloc(sizeof(FUNC_ARG_LIST));
				aux_construct = aux_construct->prox;
			}
			else {
				aux_construct->prox = NULL;
			}

			param_percorre = param_percorre->prox;
		}
	}

	// -----------------------------------------------------------------
	// insere na tabela hash:
	insert_in_hashtable(identificador, new_hashtable_val, curr_scope);
}

// ------------------------------------------------------------------------------------------------------

void assert_var_exists(HASHTABLE *curr_scope, valor_lexico *vl_identificador) {
	char* identificador = (char*)(vl_identificador->value.string);
	HASHTABLE_VALUE *valor;
	if (key_exist_in_current_or_outer_scope(identificador, curr_scope)) {
		// identificador existe. verifica se e variavel (nao vetor nem funcao):
		valor = get_value_in_current_or_outer_scope(identificador, curr_scope);
		if (is_var(valor->tipo)) {
			return;
		}
		else if (is_vec(valor->tipo)) {  // tentando usar vetor como variavel:
			printf("ERR_VECTOR: tentando usar vetor como variavel. Linha: %d\n", vl_identificador->line);
			exit(ERR_VECTOR);
		}
		else if (is_func(valor->tipo)) {  // tentando usar funcao como variavel:
			printf("ERR_FUNCTION: tentando usar funcao como variavel. Linha: %d\n", vl_identificador->line);
			exit(ERR_FUNCTION);
		}
	}
	else {
		// identificador nao existe:
		printf("ERR_UNDECLARED id nao existe linha %d\n", vl_identificador->line);
		exit(ERR_UNDECLARED);	
	}
}

void assert_vec_exists(HASHTABLE *curr_scope, valor_lexico *vl_identificador) {
	char* identificador = (char*)(vl_identificador->value.string);
	HASHTABLE_VALUE *valor;
	if (key_exist_in_current_or_outer_scope(identificador, curr_scope)) {
		// identificador existe. verifica se e vetor:
		valor = get_value_in_current_or_outer_scope(identificador, curr_scope);
		if (is_vec(valor->tipo)) {
			return;
		}
		else if (is_var(valor->tipo)) {  // tentando usar variavel como vetor:
			printf("ERR_VARIABLE: tentando usar variavel como vetor. Linha: %d\n", vl_identificador->line);
			exit(ERR_VARIABLE);
		}
		else if (is_func(valor->tipo)) {  // tentando usar funcao como vetor:
			printf("ERR_FUNCTION: tentando usar funcao como vetor. Linha: %d\n", vl_identificador->line);
			exit(ERR_FUNCTION);
		}
	}
	else {
		// identificador nao existe:
		printf("ERR_UNDECLARED id nao existe linha %d\n", vl_identificador->line);
		exit(ERR_UNDECLARED);	
	}
}

void assert_func_exists(HASHTABLE *curr_scope, valor_lexico *vl_identificador, NODE *param_list) {
	// o param_list e um func_call_list vindo do parser.y

	// verifica primeiro se existe o identificador:
	char *identificador = (char*)(vl_identificador->value.string);
	HASHTABLE_VALUE *valor;
	if (key_exist_in_current_or_outer_scope(identificador, curr_scope)) {
		// identificador existe. verifica se e funcao:
		valor = get_value_in_current_or_outer_scope(identificador, curr_scope);
		if (is_func(valor->tipo)) {
			// identificador existe e eh funcao: verifica se lista de parametros bate:
			assert_func_params(valor, param_list);
			return;
		}
		else if (is_var(valor->tipo)) {  // tentando usar variavel como funcao:
			printf("ERR_VARIABLE: tentando usar variavel como funcao. Linha: %d\n", vl_identificador->line);
			exit(ERR_VARIABLE);
		}
		else if (is_vec(valor->tipo)) {  // tentando usar vetor como funcao:
			printf("ERR_VECTOR: tentando usar vetor como funcao. Linha: %d\n", vl_identificador->line);
			exit(ERR_VECTOR);
		}
	}
	else {
		// identificador nao existe:
		printf("ERR_UNDECLARED id nao existe linha %d\n", vl_identificador->line);
		exit(ERR_UNDECLARED);	
	}
}

void assert_func_params(HASHTABLE_VALUE *valor, NODE *param_list) {
	// eu acho que isso so vai dar para fazer depois do typeinfer.
}


// ------------------------------------------------------------------------------------------------------


void assert_compatible_type_local_var_init(HASHTABLE *curr_scope, valor_lexico *vl_tipo, NODE *local_var_init_node) {
	valor_lexico *obter_pre_tipo = local_var_init_node->firstKid->data;
	int pre_tipo_localvarinit = obter_pre_tipo->type;

	int tipo_vindodahashtable;

	// ------------------------------------------
	char *tipo_escrito = vl_tipo->value.string;
	if (strcmp(tipo_escrito, "int") == 0) {
		tipo_vindodahashtable = TIPO_INT;
	}
	else if (strcmp(tipo_escrito, "float") == 0) {
		tipo_vindodahashtable = TIPO_FLOAT;
	}
	else if (strcmp(tipo_escrito, "char") == 0) {
		tipo_vindodahashtable = TIPO_CHAR;
	}
	else if (strcmp(tipo_escrito, "bool") == 0) {
		tipo_vindodahashtable = TIPO_BOOL;
	}
	else if (strcmp(tipo_escrito, "string") == 0) {
		tipo_vindodahashtable = TIPO_STRING;
	}
	// ------------------------------------------

	int tipo_localvarinit;

	HASHTABLE_VALUE *caso_precise;
	if (pre_tipo_localvarinit == ID) {  // int x <= y
		caso_precise = get_value_in_current_or_outer_scope(obter_pre_tipo->value.string, curr_scope);
		tipo_localvarinit = caso_precise->tipo;
	}
	else if (pre_tipo_localvarinit == LITERAL) {  // int x <= 10
		switch (obter_pre_tipo->litType) {
			case INT:
				tipo_localvarinit = TIPO_INT;
				break;
			case FLOAT:
				tipo_localvarinit = TIPO_FLOAT;
				break;
			case CHAR:
				tipo_localvarinit = TIPO_CHAR;
				break;
			case BOOL:
				tipo_localvarinit = TIPO_BOOL;
				break;
			case STRING:
				tipo_localvarinit = TIPO_STRING;
				break;
		}
	}

	// ------
	// verifica se tipo_vindodahashtable eh igual a tipo_localvarinit, ou se tipo_localvarinit pode ser
	// coergido para tipo_vindodahashtable.
	if (tipo_localvarinit == tipo_vindodahashtable) {
		return;
	}

	assert_allowed_coercion(tipo_localvarinit, tipo_vindodahashtable, local_var_init_node->data->line);
}

void assert_compatible_type_assignment(HASHTABLE *curr_scope, valor_lexico *vl_identificador, NODE *expression_node) {

}

void assert_integer_expression(NODE *expression_node) {

}

void assert_allowed_coercion(int from, int to, int linenum) {
	switch (from) {
		case TIPO_INT_VEC:
		case TIPO_INT_FUNC:
			from = TIPO_INT;
			break;
		case TIPO_FLOAT_VEC:
		case TIPO_FLOAT_FUNC:
			from = TIPO_FLOAT;
			break;
		case TIPO_CHAR_VEC:
		case TIPO_CHAR_FUNC:
			from = TIPO_CHAR;
			break;
		case TIPO_BOOL_VEC:
		case TIPO_BOOL_FUNC:
			from = TIPO_BOOL;
			break;
		case TIPO_STRING_VEC:
		case TIPO_STRING_FUNC:
			from = TIPO_STRING;
			break;
	}	
	switch (to) {
		case TIPO_INT_VEC:
		case TIPO_INT_FUNC:
			to = TIPO_INT;
			break;
		case TIPO_FLOAT_VEC:
		case TIPO_FLOAT_FUNC:
			to = TIPO_FLOAT;
			break;
		case TIPO_CHAR_VEC:
		case TIPO_CHAR_FUNC:
			to = TIPO_CHAR;
			break;
		case TIPO_BOOL_VEC:
		case TIPO_BOOL_FUNC:
			to = TIPO_BOOL;
			break;
		case TIPO_STRING_VEC:
		case TIPO_STRING_FUNC:
			to = TIPO_STRING;
			break;
	}

	if (from == to) {
		return;
	}

	if (from == TIPO_INT &&
		(to == TIPO_FLOAT || to == TIPO_BOOL)
		) {
		return;
	}

	if (from == TIPO_BOOL &&
		(to == TIPO_FLOAT || to == TIPO_INT)
		) {
		return;
	}

	if (from == TIPO_FLOAT &&
		(to == TIPO_INT || to == TIPO_BOOL)
		) {
		return;
	}

	if (from == TIPO_STRING) {
		printf("ERR_STRING_TO_X in line %d\n", linenum);
		exit(ERR_STRING_TO_X);
	}

	if (from == TIPO_CHAR) {
		printf("ERR_CHAR_TO_X in line %d\n", linenum);
		exit(ERR_CHAR_TO_X);
	}

	printf("ERR_WRONG_TYPE in line %d\n", linenum);
	exit(ERR_WRONG_TYPE);
}

