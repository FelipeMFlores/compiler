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

// void generate_code(void *arvore_void) {
//     if(arvore_void == NULL) return;
//     NODE *arvore = (NODE*)arvore_void;
//     printf("generate_code: %d\n", arvore->inferred_type);

//     //not sure if sibling or childrens first
//     generate_code(arvore->siblings);
//     generate_code(arvore->firstKid);

//     // TODO: add a attribute on the tree that represents the tyoe of code to be generated,
//     // example: assigment, expression, add, div, ...
//     // switch pra cada tipo, cada um vai ter uma função generate
//     // alugumas pode ser abstraidas: todas as binops vao ser iguais, soh vao mudar a operacao
//     //switch (arvore-> ){
//     //case ADD
//         generate_binop(arvore, "add");
//     //    break;
//     //}
// }

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
