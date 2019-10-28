#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

HASHTABLE *create_hashtable() {
	// Retorna uma hashtable vazia.
	HASHTABLE *res = (HASHTABLE*)malloc(sizeof(HASHTABLE));
	res->prev = NULL;
	for (int i = 0; i < HASHTABLE_SIZE; i++) {
		res->data[i] = NULL;
	}
	res->return_type = TIPO_INVALIDO; // setado em parser.y

	return res;
}

int compute_hash(char *str) {
	// *str deve ser uma string terminada em '\0'.
	// https://stackoverflow.com/questions/2624192/good-hash-function-for-strings
	int hash = 7;
	for (int i = 0; str[i] != '\0'; i++) {
		hash = hash * 31 + str[i];
	}
	if (hash < 0) {
		hash = (hash + 1) * -1;
	}
	return hash % HASHTABLE_SIZE;
}

int key_exist(char *str, HASHTABLE *hashtable) {
	// retorna se uma chave existe na hashtable.
	int hash = compute_hash(str);
	HASHTABLE_VALUE *percorre = hashtable->data[hash];
	while (percorre != NULL) {
		if (strcmp(percorre->key, str) == 0) {  // strings iguais.
			return 1;
		}
		percorre = percorre->prox;
	}
	return 0;
}

HASHTABLE_VALUE *get_value(char *str, HASHTABLE *hashtable) {
	// nao chamar get_value em hashtable que nao tem o valor.
	int hash = compute_hash(str);
	HASHTABLE_VALUE *percorre = hashtable->data[hash];
	if (percorre == NULL) {
		exit(-99);
	}
	while (strcmp(str, percorre->key)) {
		percorre = percorre->prox;
		if (percorre == NULL) {
			exit(-98);
		}
	}
	return percorre;
}

void insert_in_hashtable(char *str, HASHTABLE_VALUE *value, HASHTABLE *hashtable) {
	// nao tentar inserir chave que ja existe.
	if (key_exist(str, hashtable)) {
		exit(-97);
	}

	int hash = compute_hash(str);
	HASHTABLE_VALUE *percorre = hashtable->data[hash];
	if (percorre == NULL) {
		value->prox = NULL;
		hashtable->data[hash] = value;
		return;
	}

	while (percorre->prox != NULL) {
		percorre = percorre->prox;
	}

	value->prox = NULL;
	percorre->prox = value;
	return;
}

// funcoes para pegar no escopo atual ou algum outer-scope do atual:

int key_exist_in_current_or_outer_scope(char *str, HASHTABLE *hashtable) {
	HASHTABLE *percorre = hashtable;
	while (percorre != NULL) {
		if (key_exist(str, percorre)) {
			return 1;
		}
		percorre = percorre->prev;
	}
	return 0;
}

HASHTABLE_VALUE *get_value_in_current_or_outer_scope(char *str, HASHTABLE *hashtable) {
	// nao chamar get_value_in_current_or_outer_scope em hashtable que nao tem o valor nem
	// nela nem em hashtables mais em baixo na pilha (outer scopes).
	if (!key_exist_in_current_or_outer_scope) {
		exit(-96);
	}

	HASHTABLE *percorre = hashtable;
	while (percorre != NULL) {
		if (key_exist(str, percorre)) {
			return get_value(str, percorre);
		}
		percorre = percorre->prev;
	}

	exit(-95);
	return NULL;  // nunca executado.
}
