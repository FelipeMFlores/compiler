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

void assert_func_params(HASHTABLE_VALUE *valor, NODE *param_list, int linenum);

// --------------------------------------------------------

void assert_compatible_type_local_var_init(HASHTABLE *curr_scope, valor_lexico *vl_tipo, NODE *local_var_init_node);
void assert_compatible_type_assignment(HASHTABLE *curr_scope, valor_lexico *vl_identificador, NODE *expression_node);
void assert_integer_expression(NODE *expression_node);

// deve receber tipos do enum hashtable_value.h 
void assert_allowed_coercion(int from, int to, int linenum);

void assert_int_float_or_bool(NODE *node);

void infer_type_binop(NODE *result, NODE *op1, NODE *op2);

void set_type(NODE *node, int type);

void copy_type(NODE *from, NODE *to);

void set_type_by_vl(NODE *res, valor_lexico *vl_tipo);

void set_type_from_identifier_in_hashtable(HASHTABLE *curr_scope, NODE *res, valor_lexico *vl_identificador);

int get_clean_type(int tipo);



