#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "erros_etapa4.h"
#include "natureza.h"
#include "auxiliary.h"

HASHTABLE_VALUE *curr_vector_reading = NULL;

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

void insert_var_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, int local_var) {
	if (!curr_scope) {
		printf("exit-55\n");
		exit(-55);
	}

	char* identificador = (char*)(vl_identificador->value.string);
	if (key_exist(identificador, curr_scope)) {
		// re-declaracao de variavel no mesmo escopo:
		printf("ERR_DECLARED variable linha %d\n", vl_identificador->line);
		exit(ERR_DECLARED);
	}
	HASHTABLE_VALUE *new_hashtable_val = (HASHTABLE_VALUE*)malloc(sizeof(HASHTABLE_VALUE));
	new_hashtable_val->desloc = -1;
	new_hashtable_val->linha = vl_identificador->line;
	new_hashtable_val->natureza = NATUREZA_IDENTIFICADOR;
	new_hashtable_val->local_var = local_var;
	int i;
	for (i = 0; i < MAX_DIM; i++) {
		new_hashtable_val->dimensions_size[i] = -1;
	}

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
		printf("exit-54\n");
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
	new_hashtable_val->desloc = -1;
	int i;
	for (i = 0; i < MAX_DIM; i++) {
		new_hashtable_val->dimensions_size[i] = -1;
	}

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
		printf("exit-137\n");
		exit(-137);
	}
}

void insert_func_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, valor_lexico_list *param_list) {
	if (!curr_scope) {
		printf("exit-53\n");
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
			assert_func_params(valor, param_list, vl_identificador->line);
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

int is_allowed_coercion(int from, int to) {
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
		return 1;
	}

	if (from == TIPO_INT &&
		(to == TIPO_FLOAT || to == TIPO_BOOL)
		) {
		return 1;
	}

	if (from == TIPO_BOOL &&
		(to == TIPO_FLOAT || to == TIPO_INT)
		) {
		return 1;
	}

	if (from == TIPO_FLOAT &&
		(to == TIPO_INT || to == TIPO_BOOL)
		) {
		return 1;
	}

	if (from == TIPO_STRING) {
		return 0;
	}

	if (from == TIPO_CHAR) {
		return 0;
	}

	return 0;
}


void assert_allowed_coercion(int from, int to, int linenum) {
	int is_allowed = is_allowed_coercion(from, to);
	if (is_allowed) {
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

// --------------------------------------------------------------------------------------------------------------------------------

NODE *get_next_expression_in_func_call_list(NODE *expression_node) {
	if (expression_node->firstKid == NULL) {
		return NULL;
	}
	NODE *aux = expression_node->firstKid;
	while (aux->siblings != NULL) {
		aux = aux->siblings;
	}
	if (aux->is_another_argument == 0) {
		return NULL;
	}
	return aux;
}

void assert_func_params(HASHTABLE_VALUE *valor, NODE *param_list, int linenum) {
	// valor: HASHTABLE_VALUE da funcao.
	// param_list: func_call_list vindo do parser.y (os que passou na chamada).
	FUNC_ARG_LIST *percorre_func = valor->func_arg_list;
	NODE *percorre_params = param_list;

	// --- testes: ---
	// printf("--> parametros formais:\n");
	// while (percorre_func != NULL) {
	// 	printf("%d ", percorre_func->tipo);
	// 	percorre_func = percorre_func->prox;
	// }
	// printf("\n--> fim parametros formais.\n");

	// printf("--> argumentos passados:\n");
	// while (percorre_params != NULL) {
	// 	printf("%d ", percorre_params->inferred_type);
	// 	percorre_params = get_next_expression_in_func_call_list(percorre_params);
	// }
	// printf("\n--> fim argumentos.\n");
	// exit(-1);
	// ------------------------------------------

	int percorre_func_type;
	int percorre_params_type;

	while (percorre_func != NULL) {
		if (percorre_params == NULL) {
			printf("ERR_MISSING_ARGS line: %d\n", linenum);
			exit(ERR_MISSING_ARGS);			
		}
		percorre_func_type = get_clean_type(percorre_func->tipo);
		percorre_params_type = get_clean_type(percorre_params->inferred_type);
		
		if (is_allowed_coercion(percorre_params_type, percorre_func_type) == 0) {
			printf("ERR_WRONG_TYPE_ARGS line: %d\n", linenum);
			exit(ERR_WRONG_TYPE_ARGS);
		}
		
		percorre_func = percorre_func->prox;
		percorre_params = get_next_expression_in_func_call_list(percorre_params);
	}

	if (percorre_params != NULL) {
		printf("ERR_EXCESS_ARGS line: %d\n", linenum);
		exit(ERR_EXCESS_ARGS);
	}

}


void assert_compatible_type_assignment(HASHTABLE *curr_scope, valor_lexico *vl_identificador, NODE *expression_node) {
	char* identificador = (char*)(vl_identificador->value.string);
	HASHTABLE_VALUE *valor = get_value_in_current_or_outer_scope(identificador, curr_scope);
	assert_allowed_coercion(expression_node->inferred_type, valor->tipo, vl_identificador->line);
}

void assert_integer_expression(NODE *expression_node) {
	switch(expression_node->inferred_type) {
		case TIPO_INT:
		case TIPO_INT_VEC:
		case TIPO_INT_FUNC:
			return;
	}
	printf("ERR_WRONG_TYPE: indice do vetor nao eh inteiro. %d\n", expression_node->data->line);
	exit(ERR_WRONG_TYPE);
}

void assert_int_float_or_bool(NODE *node) {
	switch(node->inferred_type) {
		case TIPO_INT:
		case TIPO_INT_VEC:
		case TIPO_INT_FUNC:
		case TIPO_FLOAT:
		case TIPO_FLOAT_VEC:
		case TIPO_FLOAT_FUNC:
		case TIPO_BOOL:
		case TIPO_BOOL_VEC:
		case TIPO_BOOL_FUNC:
			return;
	}
	printf("ERR_WRONG_TYPE: tipo nao eh int, float nem bool. %d\n", node->data->line);
	exit(ERR_WRONG_TYPE);	
}

void infer_type_binop(NODE *result, NODE *op1, NODE *op2) {
	int tipo_result;
	int tipo_op1 = get_clean_type(op1->inferred_type);
	int tipo_op2 = get_clean_type(op2->inferred_type);

	if (tipo_op1 == TIPO_INT && tipo_op2 == TIPO_INT) {
		tipo_result = TIPO_INT;
	}
	else if (tipo_op1 == TIPO_FLOAT && tipo_op2 == TIPO_FLOAT) {
		tipo_result = TIPO_FLOAT;
	}
	else if (tipo_op1 == TIPO_BOOL && tipo_op2 == TIPO_BOOL) {
		tipo_result = TIPO_BOOL;
	}
	else if (
		(tipo_op1 == TIPO_INT && tipo_op2 == TIPO_FLOAT) ||
		(tipo_op1 == TIPO_FLOAT && tipo_op2 == TIPO_INT)
		) {
		tipo_result = TIPO_FLOAT;		
	}
	else if (
		(tipo_op1 == TIPO_INT && tipo_op2 == TIPO_BOOL) ||
		(tipo_op1 == TIPO_BOOL && tipo_op2 == TIPO_INT)
		) {
		tipo_result = TIPO_INT;		
	}
	else if (
		(tipo_op1 == TIPO_BOOL && tipo_op2 == TIPO_FLOAT) ||
		(tipo_op1 == TIPO_FLOAT && tipo_op2 == TIPO_BOOL)
		) {
		tipo_result = TIPO_FLOAT;		
	}

	result->inferred_type = tipo_result;
}

void set_type(NODE *node, int type) {
	node->inferred_type = type;
}

void copy_type(NODE *from, NODE *to) {
	to->inferred_type = from->inferred_type;
}

void set_type_by_vl(NODE *res, valor_lexico *vl_tipo) {
	int tipo_do_vl;
	char *tipo_escrito = vl_tipo->value.string;


	if (vl_tipo->type == RESERVADA) {
		if (strcmp(tipo_escrito, "int") == 0) {
			tipo_do_vl = TIPO_INT;
		}
		else if (strcmp(tipo_escrito, "float") == 0) {
			tipo_do_vl = TIPO_FLOAT;		
		}
		else if (strcmp(tipo_escrito, "char") == 0) {
			tipo_do_vl = TIPO_CHAR;
		}
		else if (strcmp(tipo_escrito, "bool") == 0) {
			tipo_do_vl = TIPO_BOOL;
		}
		else if (strcmp(tipo_escrito, "string") == 0) {
			tipo_do_vl = TIPO_STRING;		
		}
	}
	else if (vl_tipo->type == LITERAL) {
		switch(vl_tipo->litType) {
			case INT:
				tipo_do_vl = TIPO_INT;
				break;
			case FLOAT:
				tipo_do_vl = TIPO_FLOAT;
				break;
			case CHAR:
				tipo_do_vl = TIPO_CHAR;
				break;
			case BOOL:
				tipo_do_vl = TIPO_BOOL;
				break;
			case STRING:
				tipo_do_vl = TIPO_STRING;
				break;
		}
	}
	else {
		printf("**(%d\n", vl_tipo->type);


		printf("exit-23\n");
		exit(-23);
	}

	res->inferred_type = tipo_do_vl;
}

int get_clean_type(int tipo) {
	switch(tipo) {
		case TIPO_INT:
		case TIPO_INT_VEC:
		case TIPO_INT_FUNC:
			return TIPO_INT;
		case TIPO_FLOAT:
		case TIPO_FLOAT_VEC:
		case TIPO_FLOAT_FUNC:
			return TIPO_FLOAT;
		case TIPO_CHAR:
		case TIPO_CHAR_VEC:
		case TIPO_CHAR_FUNC:
			return TIPO_CHAR;
		case TIPO_BOOL:
		case TIPO_BOOL_VEC:
		case TIPO_BOOL_FUNC:
			return TIPO_BOOL;
		case TIPO_STRING:
		case TIPO_STRING_VEC:
		case TIPO_STRING_FUNC:
			return TIPO_STRING;
		case TIPO_INVALIDO:
			return TIPO_INVALIDO;
	}
	printf("exit-877");
	exit(-877);
	return 0;
}

void set_type_from_identifier_in_hashtable(HASHTABLE *curr_scope, NODE *res, valor_lexico *vl_identificador) {
	char* identificador = (char*)(vl_identificador->value.string);
	HASHTABLE_VALUE *valor = get_value_in_current_or_outer_scope(identificador, curr_scope);
	res->inferred_type = get_clean_type(valor->tipo);
}

int type_from_vl(valor_lexico *vl_tipo) {
	char *tipo_escrito = vl_tipo->value.string;
	if (strcmp(tipo_escrito, "int") == 0) {
		return TIPO_INT;
	}
	else if (strcmp(tipo_escrito, "float") == 0) {
		return TIPO_FLOAT;
	}
	else if (strcmp(tipo_escrito, "char") == 0) {
		return TIPO_CHAR;
	}
	else if (strcmp(tipo_escrito, "bool") == 0) {
		return TIPO_BOOL;
	}
	else if (strcmp(tipo_escrito, "string") == 0) {
		return TIPO_STRING;
	}
	printf("exit-67\n");
	exit(-67);
}

void assert_compatible_return_type(HASHTABLE *curr_scope, NODE *expression_node, int linenum) {
	int expression_type = get_clean_type(expression_node->inferred_type);
	HASHTABLE *percorre = curr_scope;
	int tipo_funcao_na_hashtable;
	while (percorre != NULL) {
		tipo_funcao_na_hashtable = get_clean_type(percorre->return_type);
		if (tipo_funcao_na_hashtable != TIPO_INVALIDO) {
			if (!is_allowed_coercion(expression_type, tipo_funcao_na_hashtable)) {
				printf("ERR_WRONG_PAR_RETURN Linha: %d\n", linenum);
				exit(ERR_WRONG_PAR_RETURN);
			}
			else {
				return;
			}
		}
		percorre = percorre->prev;
	}
	printf("assert_compatible_return_type: --> return sem uma funcao para retornar.\n");
	exit(-937);
}

void assert_input_param_is_identifier(NODE *expression_node) {
	valor_lexico *vl = expression_node->data;
	if (vl->type == ID) {
		return;
	}
	printf("ERR_WRONG_PAR_INPUT Linha: %d\n", vl->line);
	exit(ERR_WRONG_PAR_INPUT);
}


// ------------------------------------------------------------------------------------------------
// etapa 5:

void setAddress(HASHTABLE *curr_scope, valor_lexico *vl_identificador, int desloc) {
	char* identificador = (char*)(vl_identificador->value.string);
	if (key_exist(identificador, curr_scope) == 0) {
		printf("-- setAddress: erro 1\n");
		exit(-1);
	}
	HASHTABLE_VALUE *val = get_value(identificador, curr_scope);
	val->desloc = desloc;

	printf("-- variavel %s esta em %d\n", val->key, val->desloc);
}


int add_to_vec_decl(int n) {
	return n * INTSIZE;
}

int extract_int(valor_lexico *vl_int) {	
	int vl = vl_int->value.inteiro;
	return vl;
}

void set_curr_vector(HASHTABLE *curr_scope, valor_lexico *vl_identificador) {
	char *identificador = (char*)(vl_identificador->value.string);
	curr_vector_reading = get_value(identificador, curr_scope);
}

// set dimension size of current vector reading in parser.y.
void set_dimension_size(int dim_idx, int dim_size) {
	curr_vector_reading->dimensions_size[dim_idx] = dim_size;
	printf("-> %d, %d\n", dim_idx, dim_size);
}


