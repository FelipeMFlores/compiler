#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

struct node* newNode(struct valor_lexico *data) { 
 
  struct node* node = (struct node*)malloc(sizeof(struct node)); 
  node->n = 0;
  node->data = data; 
  node->firstKid = NULL; 
  node->siblings = NULL; 
  return(node); 
} ;



void freeTree(struct node* node){
    if(node->firstKid != NULL){
        freeTree(node->firstKid);
    }
    if(node->siblings != NULL){
        freeTree(node->siblings);
        node->siblings = NULL;
    }
    removeNode(node);
}

int removeNode(struct node* node){
    if(node->firstKid != NULL || node->siblings != NULL){
        return -1;
    }
    free(node);
    return 0;
}

void addSibling(struct node* node, struct node* newSibling){
    if(node->siblings == NULL){
        node->siblings = newSibling;
        return;
    }
    addSibling(node->siblings, newSibling);
}
void addChild(struct node* node, struct node* newChild){
    if(node->firstKid == NULL){
        node->firstKid = newChild;
        return;
    }
    addSibling(node->firstKid, newChild);

}

void printTree(struct node* node){
    printf("%d\n", node->n);
    if(node->firstKid != NULL){
        printTree(node->firstKid);
    }
    if(node->siblings != NULL){
        printTree(node->siblings);
    }
}


int main(){
    struct node *n;
    struct node *n2;
    struct node *n3;
    n = newNode(4);
    n2 = newNode(2);
    n3 = newNode(1);
    addChild(n, n2);
    addChild(n, n3);
    printTree(n);
    return 0;
}