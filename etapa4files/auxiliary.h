#pragma once

#include "hashtable.h"
#include "../tree.h"

int is_var(int tipo);
int is_vec(int tipo);
int is_func(int tipo);

void insert_var_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador);
void insert_vec_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador);
void insert_func_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, valor_lexico_list *param_list);

void assert_var_exists(HASHTABLE *curr_scope, valor_lexico *vl_identificador);
void assert_vec_exists(HASHTABLE *curr_scope, valor_lexico *vl_identificador);
void assert_func_exists(HASHTABLE *curr_scope, valor_lexico *vl_identificador, NODE *param_list);

void assert_func_params(HASHTABLE_VALUE *valor, NODE *param_list);

// --------------------------------------------------------

void assert_compatible_type_local_var_init(HASHTABLE *curr_scope, valor_lexico *vl_tipo, NODE *local_var_init_node);
void assert_compatible_type_assignment(HASHTABLE *curr_scope, valor_lexico *vl_identificador, NODE *expression_node);
void assert_integer_expression(NODE *expression_node);

// deve receber tipos do enum hashtable_value.h 
void assert_allowed_coercion(int from, int to, int linenum);


