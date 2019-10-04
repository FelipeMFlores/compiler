#include "valor_lexico.h"
struct node {
    int n;
    struct valor_lexico *data;
    struct node *firstKid;
    struct node *siblings;
};


struct node* newNode(struct valor_lexico *data);

void freeTree(struct node* node);
int removeNode(struct node* node);
void addSibling(struct node* node, struct node* newSibling);
void addChild(struct node* node, struct node* newChild);
void printTree(struct node* node);