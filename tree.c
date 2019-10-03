#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    int n;
    struct node *firstKid;
    struct node *siblings;
};

struct node* newNode(int data) 
{ 
 
  struct node* node = (struct node*)malloc(sizeof(struct node)); 
  node->n = data; 
  node->firstKid = NULL; 
  node->siblings = NULL; 
  return(node); 
} ;



int freeTree(struct node* node){
    if(node->firstKid != NULL){
        freeTree(node->firstKid);
    }
    if(node->siblings != NULL){
        freeTree(node->siblings);
        node->siblings = NULL;
    }
    
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
}