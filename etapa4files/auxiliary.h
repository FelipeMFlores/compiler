#pragma once

// tipos apenas para hashtable_value.
#define TIPO_INT 30
#define TIPO_FLOAT 31
#define TIPO_CHAR 32
#define TIPO_BOOL 33
#define TIPO_STRING 34


#include "hashtable.h"
#include "../valor_lexico.h"

void insert_var_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador);
void insert_vec_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador);
void insert_func_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, valor_lexico_list *param_list);

