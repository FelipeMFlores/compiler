#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "../tree.h"

#define MAX_SIZE 5 //1 char + 1 int

// creates a new iloc instruction, some args may be NULL
ILOC* new_iloc(char* operation, char* arg1, char* arg2, char* arg3);
//free iloc, operation and args
void free_iloc(ILOC* iloc);

NODE_LIST* new_empty_node_list();
// add iloc to next
NODE_LIST* add_iloc(NODE_LIST* list_tail, ILOC* iloc);
NODE_LIST* concat_lists(NODE_LIST* list_tail1, NODE_LIST* list_tail2);
NODE_LIST* remove_node_list(NODE_LIST* node);
// free node and iloc
void free_node_list(NODE_LIST* node);
// free entire list to both sides
void free_entire_list(NODE_LIST* node);
// free node and entire list to next
void free_entire_list_next(NODE_LIST* node);
// free node and entire list to prev
void free_entire_list_prev(NODE_LIST* node);

char* generate_label();
char* generate_register();

void generate_code(void *arvore_void);
void generate_code_rec(NODE* arvore, int short_circuit);
void generate_default(NODE *arvore);
void generate_binop(NODE *arvore, char* op);
void generate_lit_val(NODE *arvore);
void generate_ident(NODE *arvore);
void generate_assign(NODE *arvore);

void output_code(void *arvore);
void print_code(NODE* arvore);

// -------------------------------------------------------------------------------------

void setCode(NODE *node, int code);

// apenas para debug:
char* get_node_label(NODE *node);

