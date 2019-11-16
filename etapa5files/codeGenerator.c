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

ILOC* init_label() {
    char* label = generate_label();
    return new_iloc(label, NULL, NULL, NULL);
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
		case LVD:printf("LVD");break;case LVDI:printf("LVDI");break;case ASSIGN:printf("ASSIGN");break;
		case LIT_VEC_IDX:printf("LIT_VEC_IDX");break;case EXP_VEC_IDX:printf("EXP_VEC_IDX");break;
		case IF:printf("IF");break;case IF_ELSE:printf("IF_ELSE");break;case WHILE:printf("WHILE");break;
		case FOR:printf("FOR");break;case FLSC:printf("FLSC");break;case OR:printf("OR");break;case AND:printf("AND");break;
		case ADD:printf("ADD");break;case SUB:printf("SUB");break;case MULT:printf("MULT");break;case DIV:printf("DIV");break;
		case LE:printf("LE");break;case GE:printf("GE");break;case EQ:printf("EQ");break;case NEQ:printf("NEQ");break;
		case LESS:printf("LESS");break;case GREAT:printf("GREAT");break;case LITVAL:printf("LITVAL");break;
		case IDENT:printf("IDENT");break;case EXPVEC:printf("EXPVEC");break;case EXPVEC_IDX_2:printf("EXPVEC_IDX_2");break;
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
    int simple_command = 0;
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
        simple_command = 1;
        break;
    case IF_ELSE:
        simple_command = 1;
        break;
    case WHILE:
        generate_while(arvore);
        simple_command = 1;
        break;

    // OUTROS:
    case ASSIGN:
        generate_assign(arvore);
        simple_command = 1;
        break;

	// LITERAIS:
    case LITVAL:
        generate_lit_val(arvore);
		break;
    case IDENT:
        generate_ident(arvore);
        break;
	default:
		generate_default(arvore);
	}
    //concat next command. third child
    if (simple_command) {
        NODE* last_child = arvore->firstKid;
        if(last_child == NULL) return;
        last_child = last_child->siblings;
        if(last_child == NULL) return;
        last_child = last_child->siblings;
        if(last_child == NULL) return; 

        arvore->code_list = concat_lists(arvore->code_list, last_child->code_list);
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

void generate_assign(NODE *arvore) {
    // firstkid: registrador com o endereco de memoria a ser escrito.
    // firstkid->siblings: registrador com o valor a escrever.
    if(arvore == NULL) return;

    char *ident_name = arvore->firstKid->data->value.string;

    HASHTABLE_VALUE *vl = get_value_in_current_or_outer_scope(ident_name, main_scope);

    ILOC *newiloc;

    char *char_desloc = malloc(30);
    int i;
    for (i = 0; i < 30; i++) {
        char_desloc[i] = '\0';
    }
    sprintf(char_desloc, "%d", vl->desloc);

    if (vl->local_var) {
        newiloc = new_iloc("storeAI", arvore->firstKid->siblings->temp, "rfp", char_desloc);
    }
    else {
        newiloc = new_iloc("storeAI", arvore->firstKid->siblings->temp, "rbss", char_desloc);
    }

    arvore->code_list = concat_lists(arvore->firstKid->code_list, arvore->firstKid->siblings->code_list);
    arvore->code_list = add_iloc(arvore->code_list, newiloc);
}

void generate_ident(NODE *arvore) {
    if (arvore == NULL) return;
    arvore->temp = generate_register();
    char *ident_name = arvore->data->value.string;
    HASHTABLE_VALUE *val = get_value_in_current_or_outer_scope(ident_name, main_scope);

    ILOC *newiloc;

    char *char_mem_desloc = malloc(30);
    int i;
    for (i = 0; i < 30; i++)
        char_mem_desloc[i] = '\0';
    
    sprintf(char_mem_desloc, "%d", val->desloc);
    //printf("&&XX%s((\n", char_mem_desloc);

    if (val->local_var) {
        newiloc = new_iloc("loadAI", "rfp", char_mem_desloc, arvore->temp);
    }
    else {
        newiloc = new_iloc("loadAI", "rbss", char_mem_desloc, arvore->temp);
    }

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

void generate_while(NODE *arvore) {
    if (arvore == NULL) return;

    // L0: || codigo da condicao || cbr (se true, jump L1. se false, jump L2) ||
    // L1: || codigo do bloco || jumpI L0 || L2: || nop
    ILOC* label_begin = init_label();
    arvore->code_list = add_iloc(NULL, label_begin);
    // expr da condicao
    arvore->code_list = concat_lists(arvore->code_list, arvore->firstKid->code_list);
    ILOC* label_true = init_label();
    ILOC* label_false = init_label();
    // teste e jump
    ILOC* test_op = new_iloc("cbr", arvore->firstKid->temp, label_true->operation, label_false->operation);
    arvore->code_list =  add_iloc(arvore->code_list, test_op);
    // BLOCO DO WHILE
    arvore->code_list =  add_iloc(arvore->code_list, label_true);
    NODE* command_of_block = arvore->firstKid->siblings;
    while(command_of_block != NULL) {
        arvore->code_list = concat_lists(arvore->code_list, command_of_block->code_list);
        command_of_block = command_of_block->firstKid;
    }
    ILOC* jump_true = new_iloc("jumpI", label_begin->operation, NULL, NULL);
    arvore->code_list =  add_iloc(arvore->code_list, jump_true);
    // nop no final
    // LabelFalse: nop
    arvore->code_list =  add_iloc(arvore->code_list, label_false);
    ILOC* nop = new_iloc("nop", NULL, NULL, NULL);
    arvore->code_list = add_iloc(arvore->code_list, nop);
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
	// go back to the first node
	while(code->prev != NULL){
		code = code->prev;
	}
	while(code != NULL){
		ILOC* iloc = code->iloc;
		if(iloc == NULL){
			printf("iloc vazia\n");
			continue;
		} 

        printf("%s", iloc->operation);

        if (strstr(iloc->operation, "store") != NULL) {
            if (iloc->arg1) {
                printf(" %s =>", iloc->arg1);

                if (iloc->arg2) {
                    if (iloc->arg3) {
                        printf(" %s, %s\n", iloc->arg2, iloc->arg3);
                    }
                    else {
                        printf(" %s\n", iloc->arg2);
                    }
                }
                else {
                    printf("\n");
                }

            }
            else {
                printf("\n");
            }
        }
        else {
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
        }
		//printf("%s %s %s %s \n", iloc->operation, iloc->arg1, iloc->arg2, iloc->arg3);
		code = code->next;
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
		case LVD:strcpy(label,"LVD");break;case LVDI:strcpy(label,"LVDI");break;case ASSIGN:strcpy(label,"ASSIGN");break;
		case LIT_VEC_IDX:strcpy(label,"LIT_VEC_IDX");break;case EXP_VEC_IDX:strcpy(label,"EXP_VEC_IDX");break;
		case IF:strcpy(label,"IF");break;case IF_ELSE:strcpy(label,"IF_ELSE");break;case WHILE:strcpy(label,"WHILE");break;
		case FOR:strcpy(label,"FOR");break;case FLSC:strcpy(label,"FLSC");break;case OR:strcpy(label,"OR");break;case AND:strcpy(label,"AND");break;
		case ADD:strcpy(label,"ADD");break;case SUB:strcpy(label,"SUB");break;case MULT:strcpy(label,"MULT");break;case DIV:strcpy(label,"DIV");break;
		case LE:strcpy(label,"LE");break;case GE:strcpy(label,"GE");break;case EQ:strcpy(label,"EQ");break;case NEQ:strcpy(label,"NEQ");break;
		case LESS:strcpy(label,"LESS");break;case GREAT:strcpy(label,"GREAT");break;case LITVAL:strcpy(label,"LITVAL");break;
		case IDENT:strcpy(label,"IDENT");break;case EXPVEC:strcpy(label,"EXPVEC");break;case EXPVEC_IDX_2:strcpy(label,"EXPVEC_IDX_2");break;
		case GVECD:strcpy(label, "GVECD");break;
		default: strcpy(label, "??");
	}
	return label;
}