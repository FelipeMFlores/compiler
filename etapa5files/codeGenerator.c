#include "../tree.h"
#include "codeGenerator.h"


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

// -----------------------------------------------------------------------------------------------

void generate_code(void *arvore_void) {
    NODE *arvore = (NODE*)arvore_void;
    printf("generate_code: %d\n", arvore->inferred_type);
}

void output_code(void *arvore_void) {
    NODE *arvore = (NODE*)arvore_void;
    printf("generate_code: %d\n", arvore->inferred_type);
}
