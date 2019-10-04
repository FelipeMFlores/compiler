
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