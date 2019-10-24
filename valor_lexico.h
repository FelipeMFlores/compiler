#pragma once

#define RESERVADA 1
#define ESPECIAL 2
#define COMPOSTO 3
#define ID 4
#define LITERAL 5

#define NAOLIT 0
#define INT 1
#define FLOAT 2
#define CHAR 3
#define BOOL 4
#define STRING 5

union value{
    int inteiro;
    float flutuante;
    char car;
    char* boolean;
    char* string;


};
typedef struct lexico {
    int line;
    int type;
    int litType;
    union value value;
    
} valor_lexico;

typedef struct valor_lexico_list {
	valor_lexico *vl;
	struct valor_lexico_list *prox;
} valor_lexico_list;

typedef struct strlist {
    char *str;
    struct strlist *prox;
} string_list;

/**
    lista encadeada para liberar a memoria dos valor_lexico alocados
    que nao gerarao nodos na AST.
*/
extern valor_lexico_list *vl_list;

/**
    lista encadeada para liberar os strdup de strings para valores lexicos cujo value e um
    char* (boolean ou string).
*/
extern string_list *strdup_list;


void mallocVL();

void line() ;
void createReservada() ;

void createEspecial() ;

void createComposto() ;

void createID() ;

void createLitInt();


void createLitFlu();

void createLitChar();

void createLitString();

void createBool();

void insert_in_strdup_list(char *string);
