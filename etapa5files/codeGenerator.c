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
		case GVECD:printf("GVECD");break; case CONTINUE:printf("CONTINUE");break;case BREAK:printf("BREAK");break;
        case RETURN:printf("RETURN");break;case ASSIGN_VEC:printf("ASSIGN_VEC");break;
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
    int next_simple_command = 0;
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
            generate_binop(arvore, "or");
        break;
    case AND:
            generate_binop(arvore, "and");
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
        generate_if(arvore);
        next_simple_command = 3;
        break;
    case IF_ELSE:
        generate_if_else(arvore);
        next_simple_command = 4;
        break;
    case WHILE:
        generate_while(arvore);
        next_simple_command = 3;
        break;

    // OUTROS:
    case ASSIGN:
        generate_assign(arvore);
        next_simple_command = 3;
        break;
    case ASSIGN_VEC:
        generate_assign_vec(arvore);
        break;
    case LVDI:  // local var declaration com inicializacao.
        generate_lvdi(arvore);
        break;
    case EXPVEC:
        generate_expvec(arvore);
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
    if (next_simple_command--) {
        NODE* last_child = arvore->firstKid;
        if(last_child == NULL) return;
        while(next_simple_command--){
            last_child = last_child->siblings;
            if(last_child == NULL) return;
        }

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

void generate_assign_vec(NODE *arvore) {
    char *ident_name = arvore->firstKid->data->value.string;
    if (key_exist(ident_name, global_scope) == 0) {
        printf("generate_assign_vec: error 1\n");
        exit(-1);
    }
    HASHTABLE_VALUE *vl = get_value(ident_name, global_scope);

    char *desloc_rbss_char = malloc(30);
    int i;
    for (i = 0; i < 30; i++) {
        desloc_rbss_char[i] = '\0';
    }
    sprintf(desloc_rbss_char, "%d", vl->desloc);

    char* deslocs[MAX_DIM];  // deslocamentos nas dimensoes.
    int vector_number_of_dims = 0;
    for (i = 0; i < MAX_DIM; i++) {        
        if (vl->dimensions_size[i] != -1) {
            vector_number_of_dims++;
        }
        deslocs[i] = NULL;
    }

    // verifica posicoes acessar nas dimensoes:
    NODE *percorre = arvore->firstKid->firstKid;
    i = 0;
    while (percorre) {
        // pega o registrador da dimensao atual:
        deslocs[i] = percorre->temp;
        i++;
        percorre = percorre->firstKid;
        if (!percorre)
            break;
        while (percorre->siblings) {
            percorre = percorre->siblings;
        }
    }
    // for (i = 0; i < MAX_DIM && deslocs[i]; i++) {
    //     printf("%s\n", deslocs[i]);
    // }
    
    // ----------------------------------------------

    // base: rbss + desloc_rbss.
    // deslocamentos nas dimensoes: guardados em registradores em deslocs.
    // numero total de dimensoes do array: vector_number_of_dims.
    // tamanhos originais das dimensoes do array: vl->dimensions_size.

    char *base = generate_register();
    char *aux = generate_register();
    char *final_address = generate_register();

    ILOC *newiloc;

    if (vector_number_of_dims == 1) {
        // base + desloc1 * 4

        //addI    r1, c2  =>  r3    // r3 = r1 + c2
        //multI   r1, c2  =>  r3    // r3 = r1 * c2
        //mult    r1, r2  =>  r3    // r3 = r1 * r2
        //add     r1, r2  =>  r3    // r3 = r1 + r2
        newiloc = new_iloc("addI", "rbss", desloc_rbss_char, base);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
        newiloc = new_iloc("multI", deslocs[0], "4", aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
        newiloc = new_iloc("add", base, aux, final_address);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }
    else if (vector_number_of_dims == 2) {
        // base + (desloc1 * tamdim2 + desloc2) * 4

        // calcula base:
        newiloc = new_iloc("addI", "rbss", desloc_rbss_char, base); 
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // (desloc1 * tamdim2)
        newiloc = new_iloc("multI", deslocs[0], itoa(vl->dimensions_size[1]), aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // + desloc2
        newiloc = new_iloc("add", aux, deslocs[1], aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // * 4
        newiloc = new_iloc("multI", aux, "4", aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // + base
        newiloc = new_iloc("add", base, aux, final_address);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }
    else if (vector_number_of_dims > 2) {
        // calcula base:
        newiloc = new_iloc("addI", "rbss", desloc_rbss_char, base); 
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // calcula formula (resultado deve estar no registrador aux):
        multi_dimensional_formula(arvore, aux, deslocs, vl, vector_number_of_dims);

        // * 4
        newiloc = new_iloc("multI", aux, "4", aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // + base
        newiloc = new_iloc("add", base, aux, final_address);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }


    // registrador final_address contem o endereco a ser escrito.

    //store    r1 => r2        // Memoria(r2) = r1
    newiloc = new_iloc("store", arvore->firstKid->siblings->temp, final_address, NULL);
    arvore->code_list = add_iloc(arvore->code_list, newiloc);

    NODE_LIST *nlistaux = arvore->code_list;

    // firstkid: identificador do array.
    // firstkid->siblings: expression que quer escrever.
    arvore->code_list = concat_lists(arvore->firstKid->siblings->code_list, arvore->firstKid->code_list);
    arvore->code_list = concat_lists(arvore->code_list, nlistaux);
}

void generate_expvec(NODE *arvore) {
    char *ident_name = arvore->data->value.string;
    if (key_exist(ident_name, global_scope) == 0) {
        printf("generate_expvec: error 1\n");
        exit(-1);
    }   
    HASHTABLE_VALUE *vl = get_value(ident_name, global_scope);

    char *desloc_rbss_char = itoa(vl->desloc);

    int i;

    char *deslocs[MAX_DIM];
    int vector_number_of_dims = 0;
    for (i = 0; i < MAX_DIM; i++) {        
        if (vl->dimensions_size[i] != -1) {
            vector_number_of_dims++;
        }
        deslocs[i] = NULL;
    }

    NODE *percorre = arvore->firstKid;
    i = 0;
    while (percorre) {
        deslocs[i] = percorre->temp;
        i++;
        percorre = percorre->firstKid;
    }
    // for (i = 0; i < MAX_DIM && deslocs[i]; i++) {
    //      printf("%s\n", deslocs[i]);
    // }

    // ----------------------------------------------
    // base: rbss + desloc_rbss.
    // deslocamentos nas dimensoes: guardados em registradores em deslocs.
    // numero total de dimensoes do array: vector_number_of_dims.
    // tamanhos originais das dimensoes do array: vl->dimensions_size.

    char *base = generate_register();
    char *aux = generate_register();
    char *final_address = generate_register();

    ILOC *newiloc;

    if (vector_number_of_dims == 1) {
        // base + desloc1 * 4

        //addI    r1, c2  =>  r3    // r3 = r1 + c2
        //multI   r1, c2  =>  r3    // r3 = r1 * c2
        //mult    r1, r2  =>  r3    // r3 = r1 * r2
        //add     r1, r2  =>  r3    // r3 = r1 + r2
        newiloc = new_iloc("addI", "rbss", desloc_rbss_char, base);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
        newiloc = new_iloc("multI", deslocs[0], "4", aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
        newiloc = new_iloc("add", base, aux, final_address);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }
    else if (vector_number_of_dims == 2) {
        // base + (desloc1 * tamdim2 + desloc2) * 4

        // calcula base:
        newiloc = new_iloc("addI", "rbss", desloc_rbss_char, base); 
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // (desloc1 * tamdim2)
        newiloc = new_iloc("multI", deslocs[0], itoa(vl->dimensions_size[1]), aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // + desloc2
        newiloc = new_iloc("add", aux, deslocs[1], aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // * 4
        newiloc = new_iloc("multI", aux, "4", aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // + base
        newiloc = new_iloc("add", base, aux, final_address);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }
    else if (vector_number_of_dims > 2) {
        // calcula base:
        newiloc = new_iloc("addI", "rbss", desloc_rbss_char, base); 
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // calcula formula (resultado deve estar no registrador aux):
        multi_dimensional_formula(arvore, aux, deslocs, vl, vector_number_of_dims);

        // * 4
        newiloc = new_iloc("multI", aux, "4", aux);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        // + base
        newiloc = new_iloc("add", base, aux, final_address);
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }

    // ----------------------------------------------

    // registrador final_address contem o endereco de onde ler o valor.

    //load    r1       =>   r2    // r2 = Memoria(r1)

    arvore->temp = generate_register();

    newiloc = new_iloc("load", final_address, arvore->temp, NULL);
    arvore->code_list = add_iloc(arvore->code_list, newiloc);



}

void multi_dimensional_formula(NODE *arvore, char *reg_res, char *deslocs[], HASHTABLE_VALUE *vl, int vector_number_of_dims) {
    // deslocamentos nas dimensoes: guardados em registradores em deslocs.
    // numero total de dimensoes do array: vector_number_of_dims.
    // tamanhos originais das dimensoes do array: vl->dimensions_size.
    // resultado deve estar em reg_res.

    // ((((desloc1) * tamdim2 + desloc2) * tamdim3 + desloc3) * tamdim4 + desloc4) ....

    //i2i r1 => r2 // r2 = r1 para inteiros
    //multI   r1, c2  =>  r3    // r3 = r1 * c2

    ILOC *newiloc;
    newiloc = new_iloc("i2i", deslocs[0], reg_res, NULL);
    arvore->code_list = add_iloc(arvore->code_list, newiloc);

    int i;
    for (i = 1; i < vector_number_of_dims; i++) {
        newiloc = new_iloc("multI", reg_res, itoa(vl->dimensions_size[i]), reg_res); // * tamdimN
        arvore->code_list = add_iloc(arvore->code_list, newiloc);

        newiloc = new_iloc("add", reg_res, deslocs[i], reg_res); // + deslocN
        arvore->code_list = add_iloc(arvore->code_list, newiloc);
    }

}

void generate_lvdi(NODE *arvore) {
    if (arvore == NULL) return;
    char *ident_name = arvore->firstKid->siblings->data->value.string;
    HASHTABLE_VALUE *vl = get_value_in_current_or_outer_scope(ident_name, main_scope);
    ILOC *newiloc;
    //storeAI  r1       =>  r2, c3 // Memoria(r2 + c3) = r1
    newiloc = new_iloc("storeAI", arvore->firstKid->temp, "rfp", itoa(vl->desloc));
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
    if (arvore->firstKid)
        arvore->code_list = concat_lists(arvore->firstKid->code_list, arvore->code_list);
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
    if(command_of_block != NULL)
        arvore->code_list = concat_lists(arvore->code_list, command_of_block->code_list);
    ILOC* jump_begin = new_iloc("jumpI", label_begin->operation, NULL, NULL);
    arvore->code_list =  add_iloc(arvore->code_list, jump_begin);
    // nop no final
    // LabelFalse: nop
    arvore->code_list =  add_iloc(arvore->code_list, label_false);
    ILOC* nop = new_iloc("nop", NULL, NULL, NULL);
    arvore->code_list = add_iloc(arvore->code_list, nop);
}

void generate_if(NODE* arvore) {
    if (arvore == NULL) return;

    // codigo da condicao || cbr (se true, jump L1. se false, jump L2) ||
    // L1: || codigo do bloco || L2: || nop

    // expr da condicao
    arvore->code_list = concat_lists(arvore->code_list, arvore->firstKid->code_list);
    ILOC* label_true = init_label();
    ILOC* label_false = init_label();
    // teste e jump
    ILOC* test_op = new_iloc("cbr", arvore->firstKid->temp, label_true->operation, label_false->operation);
    arvore->code_list =  add_iloc(arvore->code_list, test_op);
    // BLOCO DO IF
    arvore->code_list =  add_iloc(arvore->code_list, label_true);
    NODE* command_of_block = arvore->firstKid->siblings;
    if(command_of_block != NULL) {
        arvore->code_list = concat_lists(arvore->code_list, command_of_block->code_list);
    }
    // nop no final
    // LabelFalse: nop
    arvore->code_list =  add_iloc(arvore->code_list, label_false);
    ILOC* nop = new_iloc("nop", NULL, NULL, NULL);
    arvore->code_list = add_iloc(arvore->code_list, nop);

}

void generate_if_else(NODE* arvore){
    if (arvore == NULL) return;

    // codigo da condicao || cbr (se true, jump L1. se false, jump L2) ||
    // L1: || codigo do bloco true || jump L3 || L2: || codigo do bloco else || L3: nop

    // expr da condicao
    arvore->code_list = concat_lists(arvore->code_list, arvore->firstKid->code_list);
    ILOC* label_true = init_label();
    ILOC* label_false = init_label();
    ILOC* label_end = init_label();
    
    // teste e jump
    ILOC* test_op = new_iloc("cbr", arvore->firstKid->temp, label_true->operation, label_false->operation);
    arvore->code_list =  add_iloc(arvore->code_list, test_op);
    // BLOCO DO IF true
    arvore->code_list =  add_iloc(arvore->code_list, label_true);
    NODE* command_of_block = arvore->firstKid->siblings;
    if(command_of_block != NULL) {
        arvore->code_list = concat_lists(arvore->code_list, command_of_block->code_list);
    }
    // jump pro final
    ILOC* jump_end = new_iloc("jumpI", label_end->operation, NULL, NULL);
    arvore->code_list =  add_iloc(arvore->code_list, jump_end);
    // BLOCO ELSE:
    arvore->code_list =  add_iloc(arvore->code_list, label_false);
    NODE* command_of_block_else = arvore->firstKid->siblings->siblings;
    if(command_of_block_else != NULL) {
        arvore->code_list = concat_lists(arvore->code_list, command_of_block_else->code_list);
    }
    // nop no final
    // Labelend: nop
    arvore->code_list =  add_iloc(arvore->code_list, label_end);
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
                if (strcmp(iloc->operation, "nop")) {
                    printf(":");
                }

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
		case GVECD:strcpy(label, "GVECD");break;case CONTINUE:strcpy(label, "CONTINUE");break;case BREAK:strcpy(label, "BREAK");break;
        case RETURN:strcpy(label, "RETURN");break;case ASSIGN_VEC:strcpy(label, "ASSIGN_VEC");break;
		default: strcpy(label, "??");
	}
	return label;
}