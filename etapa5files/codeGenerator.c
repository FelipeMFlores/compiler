#include <string.h>
#include "../etapa4files/hashtable.h"
#include "../etapa4files/auxiliary.h"
#include "node_code.h"
#include "codeGenerator.h"
#include "../tree.h"


// creates a new iloc instruction, some args may be NULL
ILOC* new_iloc(char* operation, char* arg1, char* arg2, char* arg3){
    ILOC* nIloc = malloc(sizeof(ILOC));
    nIloc->operation = operation;
    nIloc->arg1 = arg1;
    nIloc->arg2 = arg2;
    nIloc->arg3 = arg3;
    return nIloc;
}

void free_iloc(ILOC* iloc){
    if(iloc == NULL) 
        return;
    if(iloc->operation != NULL) free(iloc->operation);
    if(iloc->arg1 != NULL) free(iloc->arg1);
    if(iloc->arg2 != NULL) free(iloc->arg2);
    if(iloc->arg3 != NULL) free(iloc->arg3);

    free(iloc);    
    iloc = NULL;
}

NODE_LIST* new_empty_node_list(){
    NODE_LIST* new_node = malloc(sizeof(NODE_LIST));
    new_node->iloc = NULL;
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

NODE_LIST* add_iloc(NODE_LIST* list_tail, ILOC* iloc){
    NODE_LIST* new_node = new_empty_node_list();
    new_node->iloc = iloc;

    if(list_tail != NULL){
        list_tail->next = new_node;
        new_node->prev = list_tail;
    }
    return new_node;
}

NODE_LIST* concat_lists(NODE_LIST* list_tail1, NODE_LIST* list_tail2){
    if(list_tail1 == NULL) return list_tail2;
	if(list_tail2 == NULL) return list_tail1;
    NODE_LIST* second_list_first = list_tail2;
    while(second_list_first->prev != NULL){
        second_list_first = second_list_first->prev;
    }
    list_tail1->next = second_list_first;
    second_list_first->prev = list_tail1;

	return list_tail2;
}

void free_node_list(NODE_LIST* node){
    if(node == NULL) return;

    if(node->iloc != NULL) {
        free_iloc(node->iloc);
        node->iloc = NULL;
    }
    node->next = NULL;
    node->prev = NULL;
    free(node);
    node = NULL;
}

void free_entire_list(NODE_LIST* node){
    if(node == NULL) return;
    free_entire_list_next(node->next);
    free_entire_list_prev(node->prev);
    free_node_list(node);
}

void free_entire_list_next(NODE_LIST* node){
    if(node == NULL) return;
    free_entire_list_next(node->next);
    free_node_list(node);
}

void free_entire_list_prev(NODE_LIST* node){
   if(node == NULL) return;
    free_entire_list_prev(node->prev);
    free_node_list(node);
}


char* generate_label(){
    static int label_number = 0;
    char* new_label = malloc(MAX_SIZE); 
    snprintf(new_label, MAX_SIZE, "L%d", label_number++);
    return new_label;
}

char* generate_register(){
    static int register_number = 0;
    char* new_register = malloc(MAX_SIZE); 
    snprintf(new_register, MAX_SIZE, "r%d", register_number++);
    return new_register;
}

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



void generate_code(void *arvore_void) {
    if(arvore_void == NULL) return;
    NODE *arvore = (NODE*)arvore_void;
    global_scope = main_scope->prev;

	generate_code_rec(arvore, 0);
	print_code(arvore);

}

void generate_code_rec(NODE* arvore, int short_circuit) {
	if(arvore == NULL) return;

    int need_short_circuit = 0;
    if (short_circuit) {
        need_short_circuit = 1;
    }
    else {
        if (arvore->code == IF || arvore->code == IF_ELSE || arvore->code == WHILE ||
            arvore->code == FOR) {
            need_short_circuit = 1;
        }
    }

    //not sure if sibling or childrens first
    generate_code_rec(arvore->firstKid, need_short_circuit);
    generate_code_rec(arvore->siblings, need_short_circuit);

	switch (arvore->code){
	// OPERADORES:
    case ADD:
        generate_binop(arvore, "add");
       break;
    case SUB:
        generate_binop(arvore, "sub");
        break;
    case MULT:
        generate_binop(arvore, "mult");
        break;
    case DIV:
        generate_binop(arvore, "div");
        break;
    case OR:
        if (!need_short_circuit)
            generate_binop(arvore, "or");
        else {

        }
        break;
    case AND:
        if (!need_short_circuit)
            generate_binop(arvore, "and");
        else {
            
        }
        break;
	// RELACIONAIS:
	case LE:
        generate_binop(arvore, "cmp_LE");
       	break;
	case GE:
        generate_binop(arvore, "cmp_GE");
       	break;
	case EQ:
        generate_binop(arvore, "cmp_EQ");
       	break;
	case NEQ:
        generate_binop(arvore, "cmp_NE");
       	break;
	case LESS:
        generate_binop(arvore, "cmp_LT");
       	break;
	case GREAT:
        generate_binop(arvore, "cmp_GT");
       	break;

    // CONDICIONAIS:
    case IF:
        break;
    case IF_ELSE:
        break;
    case WHILE:
        break;
    case FOR:
        break;

	// LITERAIS:
    case LITVAL:
        generate_lit_val(arvore);
		break;
	default:
		generate_default(arvore);
	}
}

void generate_binop(NODE *arvore, char* op){
    if(arvore == NULL) return;
    arvore->temp = generate_register();
    //at this point, both childrens already are generated
    // op  e1.temp, e2.temp => e.temp
    ILOC* newiloc = new_iloc(op, arvore->firstKid->temp, arvore->firstKid->siblings->temp, arvore->temp);
	arvore->code_list = concat_lists(arvore->firstKid->code_list, arvore->firstKid->siblings->code_list);

    //add to e.code
    arvore->code_list = add_iloc(arvore->code_list, newiloc);
}

void generate_lit_val(NODE *arvore) {
    if (arvore == NULL) return;
    arvore->temp = generate_register();
    int i, j = arvore->data->value.inteiro;
    arvore->data->value.string = malloc(30);
    for (i = 0; i < 30; i++) arvore->data->value.string[i] = '\0';
    sprintf(arvore->data->value.string, "%d", j);
    ILOC *newiloc = new_iloc("loadI", arvore->data->value.string, arvore->temp, NULL);
    //printf("$$$$%s**\n", arvore->data->value.string);
    arvore->code_list = add_iloc(arvore->code_list, newiloc);
}

// joga pra cima o codigo
void generate_default(NODE *arvore) {
	if(arvore == NULL || arvore->firstKid == NULL) return;
	NODE* kid = arvore->firstKid;
	while(kid != NULL) {
		arvore->code_list = concat_lists(arvore->code_list, kid->code_list);
		kid = kid->siblings;
	}
}

//for testing
void print_code(NODE* arvore) {
	if(arvore == NULL) {
		printf("Arvore Vazia\n");
		return;
	}
	NODE_LIST* code = arvore->code_list;
	if(code == NULL){
		printf("Codigo Vazio\n");
		return;
	}
	while(code != NULL){
		ILOC* iloc = code->iloc;
		if(iloc == NULL){
			printf("iloc vazia\n");
			continue;
		} 

        printf("%s", iloc->operation);
        if (iloc->arg1) {
            printf(" %s", iloc->arg1);

            if (iloc->arg2) {
                if (iloc->arg3) {
                    printf(", %s => %s\n", iloc->arg2, iloc->arg3);
                }
                else {
                    printf(" => %s\n", iloc->arg2);
                }
            }
            else {
                printf("\n");
            }

        }
        else {
            printf("\n");
        }
		//printf("%s %s %s %s \n", iloc->operation, iloc->arg1, iloc->arg2, iloc->arg3);
		code = code->prev;
	}
}

void output_code(void *arvore_void) {
    NODE *arvore = (NODE*)arvore_void;
    printf("generate_code: %d\n", arvore->inferred_type);
}

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