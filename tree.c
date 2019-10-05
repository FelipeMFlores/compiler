#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

NODE* newNode(valor_lexico *data) { 
 
  NODE* node = (NODE*)malloc(sizeof(NODE)); 
  node->n = 0;
  node->data = data; 
  node->firstKid = NULL; 
  node->siblings = NULL; 
  node->parent = NULL;
  return(node); 
} ;



void freeTree(NODE* node){
    if(node->firstKid != NULL){
        freeTree(node->firstKid);
    }
    if(node->siblings != NULL){
        freeTree(node->siblings);
        node->siblings = NULL;
    }
    removeNode(node);
}

int removeNode(NODE* node){
    if(node->firstKid != NULL || node->siblings != NULL){
        return -1;
    }
    free(node);
    return 0;
}

void addSibling(NODE* node, NODE* newSibling){
    if (newSibling == NULL) {
        return;
    }
    if (node == NULL) {
        return;
    }

    if(node->siblings == NULL){
        node->siblings = newSibling;
        newSibling->parent = node->parent;
        return;
    }
    addSibling(node->siblings, newSibling);
}
void addChild(NODE* node, NODE* newChild){
    if (newChild == NULL) {
        return;
    }
    if (node == NULL) {
        return;
    }

    if(node->firstKid == NULL){
        node->firstKid = newChild;
        newChild->parent = node;
        return;
    }
    addSibling(node->firstKid, newChild);

}

void printTree(void* root){
    NODE* arvore = root;
    if (root == NULL) {
        printf("--> printTree: Arvore vazia.\n");
        return;
    }
    printTree2(arvore, 1);
}

void printTree2(NODE* node, int lvl){
    for(int i = 0; i < 3*lvl; i++)
        printf("-");
    if(node->data->type == ESPECIAL || node->data->litType == CHAR){
        printf(" %c\n", node->data->value);
    }else {
        switch (node->data->litType)
        {
        case NAOLIT:
        case BOOL:
        case STRING:
            printf(" %s\n", node->data->value);
            break;
        case INT:
            printf(" %d\n", node->data->value.inteiro);
            break;
        case FLOAT:
            printf(" %f\n", node->data->value.flutuante);
            break;
        }
    }
    if(node->firstKid != NULL){
        printTree2(node->firstKid, lvl+1);
    }
    if(node->siblings != NULL){
        printTree2(node->siblings, lvl);
}
}

// ----------------------------------------------------------------------------------------

void libera (void *arvore) {
    if (arvore == NULL) {
        printf("--> libera: Arvore vazia.\n");
        return;
    }
    libera_mallocVLs();
    libera_recursivo((NODE*)arvore);
}

void libera_mallocVLs() {
    valor_lexico_list *percorre = vl_list;
    valor_lexico_list *next = vl_list->prox;

    while (next != NULL) {
        free(percorre->vl);
        free(percorre);
        percorre = next;
        next = next->prox;
    }
    free(percorre->vl);
    free(percorre);
}

void deleta_nodo(NODE* node) {
    free(node);
}

void libera_recursivo(NODE* node) {
    if (node->firstKid) {
        libera_recursivo(node->firstKid);
    }
    if (node->siblings) {
        libera_recursivo(node->siblings);
    }
    deleta_nodo(node);
}

void exporta (void *arvore) {
    FILE *csv = fopen("e3.csv", "w");
    if (arvore == NULL) {
        printf("--> exporta: Arvore vazia. \"e3.csv\" vazio criado.\n");
    }
    else {
        exporta_recursivo((NODE*)arvore, csv);
    }
    fclose(csv);    
}

void exporta_recursivo(NODE *node, FILE *csv) {
    if (node == NULL) {
        return;
    }
    if (node->firstKid != NULL) {
        fprintf(csv, "%p, %p\n", (void*)node, (void*)(node->firstKid));
        exporta_recursivo(node->firstKid, csv);
    }
    if (node->siblings != NULL) {
        fprintf(csv, "%p, %p\n", (void*)node->parent, (void*)(node->siblings));
        exporta_recursivo(node->siblings, csv);        
    }
}
