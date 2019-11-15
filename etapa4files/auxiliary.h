#pragma once

#include "hashtable.h"
#include "../tree.h"

#define INTSIZE 4

int is_var(int tipo);
int is_vec(int tipo);
int is_func(int tipo);

void insert_var_decl(HASHTABLE *curr_scope, valor_lexico *vl_tipo, valor_lexico *vl_identificador, int local_var);
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
int is_allowed_coercion(int from, int to);
void assert_allowed_coercion(int from, int to, int linenum);

void assert_int_float_or_bool(NODE *node);

void infer_type_binop(NODE *result, NODE *op1, NODE *op2);

void set_type(NODE *node, int type);

void copy_type(NODE *from, NODE *to);

void set_type_by_vl(NODE *res, valor_lexico *vl_tipo);

void set_type_from_identifier_in_hashtable(HASHTABLE *curr_scope, NODE *res, valor_lexico *vl_identificador);

int get_clean_type(int tipo);

// pega tipo de valor lexico 'type' do parser.y
int type_from_vl(valor_lexico *vl_tipo);

NODE* get_next_expression_in_func_call_list(NODE *expression_node);

void assert_compatible_return_type(HASHTABLE *curr_scope, NODE *expression_node, int linenum);

// --------------------------------------------------------

void assert_input_param_is_identifier(NODE *expression_node);


// ---------------------------------------------------------
// etapa 5:

void setAddress(HASHTABLE *curr_scope, valor_lexico *vl_identificador, int desloc);

int add_to_vec_decl(valor_lexico *vl_intlit);
