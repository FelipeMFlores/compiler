#include <string.h>
#include "../etapa4files/hashtable.h"
#include "../etapa4files/auxiliary.h"
#include "node_code.h"
#include "codeGenerator.h"


// // creates a new iloc instruction, some args may be NULL
// ILOC* new_iloc(char* operation, char* arg1, char* arg2, char* arg3){
//     ILOC* nIloc = malloc(sizeof(ILOC));
//     nIloc->operation = operation;
//     nIloc->arg1 = arg1;
//     nIloc->arg2 = arg2;
//     nIloc->arg3 = arg3;
//     return nIloc;
// }

// void free_iloc(ILOC* iloc){
//     if(iloc == NULL) 
//         return;
//     if(iloc->operation != NULL) free(iloc->operation);
//     if(iloc->arg1 != NULL) free(iloc->arg1);
//     if(iloc->arg2 != NULL) free(iloc->arg2);
//     if(iloc->arg3 != NULL) free(iloc->arg3);

//     free(iloc);    
//     iloc = NULL;
// }

// NODE_LIST* new_empty_node_list(){
//     NODE_LIST* new_node = malloc(sizeof(NODE_LIST));
//     new_node->iloc = NULL;
//     new_node->prev = NULL;
//     new_node->next = NULL;
//     return new_node;
// }

// NODE_LIST* add_iloc(NODE_LIST* list_tail, ILOC* iloc){
//     NODE_LIST* new_node = new_empty_node_list();
//     new_node->iloc = iloc;

//     if(list_tail != NULL){
//         list_tail->next = new_node;
//         new_node->prev = list_tail;
//     }
//     return new_node;
// }

// void concat_lists(NODE_LIST* list_tail1, NODE_LIST* list_tail2){
//     if(list_tail1 == NULL || list_tail2 == NULL) return;
//     NODE_LIST* second_list_first = list_tail2;
//     while(second_list_first->prev != NULL){
//         second_list_first = second_list_first->prev;
//     }
//     list_tail1->next = second_list_first;
//     second_list_first->prev = list_tail1;
// }

// void free_node_list(NODE_LIST* node){
//     if(node == NULL) return;

//     if(node->iloc != NULL) {
//         free_iloc(node->iloc);
//         node->iloc = NULL;
//     }
//     node->next = NULL;
//     node->prev = NULL;
//     free(node);
//     node = NULL;
// }

// void free_entire_list(NODE_LIST* node){
//     if(node == NULL) return;
//     free_entire_list_next(node->next);
//     free_entire_list_prev(node->prev);
//     free_node_list(node);
// }

// void free_entire_list_next(NODE_LIST* node){
//     if(node == NULL) return;
//     free_entire_list_next(node->next);
//     free_node_list(node);
// }

// void free_entire_list_prev(NODE_LIST* node){
//    if(node == NULL) return;
//     free_entire_list_prev(node->prev);
//     free_node_list(node);
// }


// char* generate_label(){
//     static int label_number = 0;
//     char* new_label = malloc(MAX_SIZE); 
//     snprintf(new_label, MAX_SIZE, "L%d", label_number++);
//     return new_label;
// }

// char* generate_register(){
//     static int register_number = 0;
//     char* new_register = malloc(MAX_SIZE); 
//     snprintf(new_register, MAX_SIZE, "r%d", register_number++);
//     return new_register;
// }

// // -----------------------------------------------------------------------------------------------

void print_ncode(NODE *node) {
	printf("print_ncode: %p ", (void*)(node));
	switch(node->code) {
		case INVALID_CODE:printf("INVALID_CODE");break;case GVD:printf("GVD");break;
		case LVD:printf("LVD");break;case LVDI:printf("LVDI");case ASSIGN:printf("ASSIGN");
		case LIT_VEC_IDX:printf("LIT_VEC_IDX");break;case EXP_VEC_IDX:printf("EXP_VEC_IDX");
		case IF:printf("IF");break;case IF_ELSE:printf("IF_ELSE");case WHILE:printf("WHILE");
		case FOR:printf("FOR");break;case FLSC:printf("FLSC");case OR:printf("OR");case AND:printf("AND");
		case ADD:printf("ADD");break;case SUB:printf("SUB");case MULT:printf("MULT");case DIV:printf("DIV");
		case LE:printf("LE");break;case GE:printf("GE");case EQ:printf("EQ");case NEQ:printf("NEQ");
		case LESS:printf("LESS");break;case GREAT:printf("GREAT");case LITVAL:printf("LITVAL");
		case IDENT:printf("IDENT");break;case EXPVEC:printf("EXPVEC");case EXPVEC_IDX_2:printf("EXPVEC_IDX_2");
		case GVECD:printf("GVECD");break;
		default: printf("??");
	}
	printf("\n");
}


int next_local_address = 0;
int next_global_address = 0;

#define INTSIZE 4

void compute_addresses(NODE *arvore) {
	//print_ncode(arvore);
	// -------------
	if (global_scope == NULL) {
		global_scope = main_scope->prev;
	}

	HASHTABLE_VALUE *aux;

	NODE *aux_vec;

	int add_inc;

	if (arvore->code == GVD) {  // global var declaration.
		if (key_exist(arvore->data->value.string, global_scope) == 0) {
			printf("compute_addresses: erro 1\n");
			exit(-1);
		}
		aux = get_value(arvore->data->value.string, global_scope);
		if (aux->desloc == -1) {
			aux->desloc = next_global_address;
			next_global_address += INTSIZE;
			printf("-> GVD %s in %d\n", aux->key, aux->desloc);
		}
	}

	else if (arvore->code == LVD || arvore->code == LVDI) {  // local var declaration sem ou com inicialização.
		if (key_exist(arvore->data->value.string, main_scope) == 0) {
			printf("compute_addresses: erro 2\n");
			exit(-1);
		}
		aux = get_value(arvore->data->value.string, main_scope);
		if (aux->desloc == -1) {
			aux->desloc = next_local_address;
			next_local_address += INTSIZE;
			if (arvore->code == LVD)
				printf("-> LVD %s in %d\n", aux->key, aux->desloc);
			else
				printf("-> LVDI %s in %d\n", aux->key, aux->desloc);
		}
	}

	else if (arvore->code == GVECD) {  // global vector declaration.
		if (key_exist(arvore->data->value.string, global_scope) == 0) {
			printf("compute_Addresses: erro 3\n");
			exit(-1);
		}
		aux = get_value(arvore->data->value.string, global_scope);
	
		printf("$$$ %d\n", aux->desloc);

		if (aux->desloc == -1) {
			aux->desloc = next_global_address;
			printf("-> GVECD %s in %d\n", aux->key, aux->desloc);
			// calcula proximo endereço disponivel:
			add_inc = 0;

			aux_vec = arvore->firstKid;
			while (aux_vec) {
				printf("** %d\n", aux_vec->data->value.inteiro);
				aux_vec = aux_vec->firstKid;
			}

		}
	}




	// -----------------
	if (arvore->firstKid != NULL) {
		compute_addresses(arvore->firstKid);
	}
	NODE *sib = arvore->siblings;
	while (sib) {
		compute_addresses(sib);
		sib = sib->siblings;
	}
}

void generate_code(void *arvore_void) {
    if(arvore_void == NULL) return;
    NODE *arvore = (NODE*)arvore_void;
    compute_addresses(arvore);



    //not sure if sibling or childrens first
    // generate_code(arvore->siblings);
    // generate_code(arvore->firstKid);

    // TODO: add a attribute on the tree that represents the tyoe of code to be generated,
    // example: assigment, expression, add, div, ...
    // switch pra cada tipo, cada um vai ter uma função generate
    // alugumas pode ser abstraidas: todas as binops vao ser iguais, soh vao mudar a operacao
    //switch (arvore-> ){
    //case ADD
        // generate_binop(arvore, "add");
    //    break;
    //}
}

// void generate_binop(NODE *arvore, char* op){
//     if(arvore == NULL) return;
//     //arvore->temp = generate_register();
//     //at this point, both childrens already are generated
//     // op  e1.temp, e2.temp => e.temp
//     //ILOC* newiloc = new_iloc(op, arvore->firstKid->temp, arvore->firstKid->siblings->temp, arvore->temp);
//     concat_lists(arvore->firstKid->code, arvore->firstKid->siblings->code);

//     //add to e.code
//     arvore->code = add_iloc(arvore->firstKid->siblings->code, new_iloc);
// }
// void output_code(void *arvore_void) {
//     NODE *arvore = (NODE*)arvore_void;
//     printf("generate_code: %d\n", arvore->inferred_type);
// }


void setCode(NODE *node, int code) {
    node->code = code;
}


char* get_node_label(NODE *node) {
	char *label = malloc(40);
	for (int i = 0; i < 40; i++) {
		label[i] = '\0';
	}
	switch(node->code) {
		case INVALID_CODE:strcpy(label,"INVALID_CODE");break;case GVD:strcpy(label,"GVD");break;
		case LVD:strcpy(label,"LVD");break;case LVDI:strcpy(label,"LVDI");case ASSIGN:strcpy(label,"ASSIGN");
		case LIT_VEC_IDX:strcpy(label,"LIT_VEC_IDX");break;case EXP_VEC_IDX:strcpy(label,"EXP_VEC_IDX");
		case IF:strcpy(label,"IF");break;case IF_ELSE:strcpy(label,"IF_ELSE");case WHILE:strcpy(label,"WHILE");
		case FOR:strcpy(label,"FOR");break;case FLSC:strcpy(label,"FLSC");case OR:strcpy(label,"OR");case AND:strcpy(label,"AND");
		case ADD:strcpy(label,"ADD");break;case SUB:strcpy(label,"SUB");case MULT:strcpy(label,"MULT");case DIV:strcpy(label,"DIV");
		case LE:strcpy(label,"LE");break;case GE:strcpy(label,"GE");case EQ:strcpy(label,"EQ");case NEQ:strcpy(label,"NEQ");
		case LESS:strcpy(label,"LESS");break;case GREAT:strcpy(label,"GREAT");case LITVAL:strcpy(label,"LITVAL");
		case IDENT:strcpy(label,"IDENT");break;case EXPVEC:strcpy(label,"EXPVEC");case EXPVEC_IDX_2:strcpy(label,"EXPVEC_IDX_2");
		case GVECD:strcpy(label, "GVECD");break;
		default: strcpy(label, "??");
	}
	return label;
}