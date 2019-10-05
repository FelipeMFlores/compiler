#include "valor_lexico.h"
typedef struct node {
    int n;
    struct lexico *data;
    struct node *firstKid;
    struct node *siblings;
} NODE;


NODE* newNode(valor_lexico *data);

void freeTree( NODE* node);
int removeNode( NODE* node);
void addSibling( NODE* node, NODE* newSibling);
void addChild( NODE* node, NODE* newChild);
void printTree(void *root);
void printTree2(NODE* node, int lvl);