#pragma once

#include "hashtable_value.h"

#define HASHTABLE_SIZE 300

typedef struct hashtable {
	HASHTABLE_VALUE* data[HASHTABLE_SIZE];
	struct hashtable *prev;  // Para implementar stack de hashtables.
} HASHTABLE;

HASHTABLE *create_hashtable();

int compute_hash(char *str);
int key_exist(char *str, HASHTABLE *hashtable);
HASHTABLE_VALUE *get_value(char *str, HASHTABLE *hashtable);
void insert_in_hashtable(char *str, HASHTABLE_VALUE *value, HASHTABLE *hashtable);
int key_exist_in_current_or_outer_scope(char *str, HASHTABLE *hashtable);
HASHTABLE_VALUE *get_value_in_current_or_outer_scope(char *str, HASHTABLE *hashtable);
