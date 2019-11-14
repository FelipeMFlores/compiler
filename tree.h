#pragma once

#include <stdio.h>
#include "valor_lexico.h"
typedef struct node {
    int n;
    struct lexico *data;
    struct node *firstKid;
    struct node *siblings;
    struct node *parent;
    //char* temp
    //NODE_LIST* code

    int inferred_type;

    // criado para conseguir fazer assert_func_params.
    int is_another_argument;

    // codigo do nodo para geração de código:
    int code;

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
