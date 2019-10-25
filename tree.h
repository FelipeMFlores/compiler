#pragma once


#include "valor_lexico.h"
typedef struct node {
    int n;
    struct lexico *data;
    struct node *firstKid;
    struct node *siblings;
    struct node *parent;


    int inferred_type;

} NODE;

typedef struct nlist {
	NODE *nodo;
	struct nlist *prox;
} node_list;

/**
	lista encadeada auxiliar para liberar memoria dos nodos.
*/
extern node_list* nodes_list;

NODE* newNode(valor_lexico *data);

valor_lexico_list* newValorLexicoList(valor_lexico *tipo);


void freeTree( NODE* node);
int removeNode( NODE* node);
void addSibling( NODE* node, NODE* newSibling);
void addChild( NODE* node, NODE* newChild);
// void printTree(void *root);
// void printTree2(NODE* node, int lvl);

void exporta_recursivo(NODE *node, FILE *csv);
void libera_recursivo(NODE* node);
void deleta_nodo(NODE *node);
void libera_nodos();
void libera_mallocVLs();
void libera_strdups();
