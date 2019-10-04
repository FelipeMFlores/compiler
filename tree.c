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
    if(node->siblings == NULL){
        node->siblings = newSibling;
        return;
    }
    addSibling(node->siblings, newSibling);
}
void addChild(NODE* node, NODE* newChild){
    if(node->firstKid == NULL){
        node->firstKid = newChild;
        return;
    }
    addSibling(node->firstKid, newChild);

}

void printTree(void* root){
    NODE* arvore = root;
    printTree2(arvore);
}

void printTree2(NODE* node){
        printf(" a %s\n", node->data->value);
    if(node->firstKid != NULL){
        printTree2(node->firstKid);
    }
    if(node->siblings != NULL){
        printTree2(node->siblings);
}
}