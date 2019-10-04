
#define RESERVADA 1
#define ESPECIAL 2
#define COMPOSTO 3
#define ID 4
#define LITERAL 5

union value{
    int inteiro;
    float flutuante;
    char car;
    int boolean;
    char* cadeia;


};
struct valor_lexico {
    int line;
    int type;
    union value value;
    
};



void mallocVL();

void line() ;
void createReservada() ;

void createEspecial() ;

void createComposto() ;

void createID() ;

void createLInt();
