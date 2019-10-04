%{
#include "parser.tab.h"
#include <string.h>
#include "valor_lexico.h"

int line_counter = 1;

%}

letra [a-z]|[A-Z]|_
numero [0-9]
id {letra}({letra}|{numero})*
especialChar ["," ";" ":" "(" ")" \] \[ "{" "}" "+" \- "|" "?" "@" "*" "/" "<" ">" "=" "!" "&" "%" "#" "^" "." "$" "~" "`"]

litInt {numero}+

rawFloat {litInt}("."){numero}+
sciFloat ({litInt}|{rawFloat})("e"|"E"){litInt}
litFloat {rawFloat}|{sciFloat}

litChar '({letra}|{numero}|{especialChar})'

litString \"[^\"]*\"

%x COMMENT

%%

"//".*					//skip single line comment

"/*"					{ BEGIN(COMMENT)			; }
<COMMENT>"*/" 			{ BEGIN(INITIAL)			; }
<COMMENT>(.) 			;
<COMMENT>("\n")			line_counter++;
<COMMENT><<EOF>> 		{ return TOKEN_ERRO			; }

" "+					//skip whitespace
"\t"+					//skip tabs
int						{ createReservada(); return TK_PR_INT          ; }
float					{ createReservada(); return TK_PR_FLOAT        ; }  
bool					{ createReservada(); return TK_PR_BOOL         ; }  
char					{ createReservada(); return TK_PR_CHAR         ; }  
string					{ createReservada(); return TK_PR_STRING       ; }  
if						{ createReservada(); return TK_PR_IF           ; }  
then					{ createReservada(); return TK_PR_THEN         ; } 
else					{ createReservada(); return TK_PR_ELSE         ; }  
while					{ createReservada(); return TK_PR_WHILE        ; }  
do						{ createReservada(); return TK_PR_DO           ; }  
input					{ createReservada(); return TK_PR_INPUT        ; }  
output					{ createReservada(); return TK_PR_OUTPUT       ; }  
return					{ createReservada(); return TK_PR_RETURN       ; }  
const					{ createReservada(); return TK_PR_CONST        ; }  
static					{ createReservada(); return TK_PR_STATIC       ; }  
foreach					{ createReservada(); return TK_PR_FOREACH      ; }  
for						{ createReservada(); return TK_PR_FOR          ; } 
switch					{ createReservada(); return TK_PR_SWITCH       ; }  
case					{ createReservada(); return TK_PR_CASE         ; }  
break					{ createReservada(); return TK_PR_BREAK        ; }  
continue				{ createReservada(); return TK_PR_CONTINUE     ; }  
class					{ createReservada(); return TK_PR_CLASS        ; }  
private					{ createReservada(); return TK_PR_PRIVATE      ; }  
public					{ createReservada(); return TK_PR_PUBLIC       ; }  
protected				{ createReservada(); return TK_PR_PROTECTED    ; }  

false					{ createBool(); return TK_LIT_FALSE		; }
true 					{ createBool(); return TK_LIT_TRUE        ; }

"<="					{ createComposto(); return TK_OC_LE           ; }  
">="					{ createComposto(); return TK_OC_GE           ; } 
"=="					{ createComposto(); return TK_OC_EQ           ; }  
"!="					{ createComposto(); return TK_OC_NE           ; }  
"&&"					{ createComposto(); return TK_OC_AND          ; }  
"||"					{ createComposto(); return TK_OC_OR           ; }  
">>"					{ createComposto(); return TK_OC_SL           ; }  
"<<"					{ createComposto(); return TK_OC_SR           ; }  
"%>%"					{ createComposto(); return TK_OC_FORWARD_PIPE ; }  
"%|%"					{ createComposto(); return TK_OC_BASH_PIPE    ; }  
end						{ createReservada(); return TK_PR_END          ; } 
default					{ createReservada(); return TK_PR_DEFAULT      ; }
{id}					{ createID(); 		return TK_IDENTIFICADOR	; }
{especialChar}			{ createEspecial(); return yytext[0]			; }

{litInt}				{ createLitInt(); return TK_LIT_INT			; }
{litFloat}				{ createLitFlu(); return TK_LIT_FLOAT		; }
{litChar}				{ createLitChar(); return TK_LIT_CHAR		; }
{litString}				{ createLitString(); return TK_LIT_STRING		; }

"\n"					line_counter++;



({litInt}|{litFloat})({id})     { return TOKEN_ERRO ;}
\'[^(\'|{letra}|{numero}|{especialChar})]*\'	{ return TOKEN_ERRO ;}
\'{id}\'				{ return TOKEN_ERRO 		; }

.						{ return TOKEN_ERRO			; }

%%


int get_line_number () {
	return line_counter;
}

void mallocVL(){
	yylval.valor_lexico = (valor_lexico*) malloc(sizeof(valor_lexico));
}

void line() {
	yylval.valor_lexico->line = get_line_number();
}
void createReservada() {
	mallocVL();
	line();
	yylval.valor_lexico->type = RESERVADA;
	yylval.valor_lexico->litType = NAOLIT;
	yylval.valor_lexico->value.string = strdup(yytext);
}

void createEspecial() {
	mallocVL();
	line();
	yylval.valor_lexico->type = ESPECIAL;
	yylval.valor_lexico->litType = NAOLIT;
	yylval.valor_lexico->value.car = yytext[0]; // ultimo char lido
}

void createComposto() {
	mallocVL();
	line();
	yylval.valor_lexico->type = COMPOSTO;
	yylval.valor_lexico->litType = NAOLIT;
	yylval.valor_lexico->value.string = strdup(yytext); 
}

void createID() {
	mallocVL();
	line();
	yylval.valor_lexico->type = ID;
	yylval.valor_lexico->litType = NAOLIT;
	yylval.valor_lexico->value.string = strdup(yytext);
}

void createLitInt() {
	mallocVL();
	line();
	yylval.valor_lexico->type = LITERAL;
	yylval.valor_lexico->litType = INT;
	char* number = strdup(yytext);
	yylval.valor_lexico->value.inteiro = atoi(number); 
}

void createLitFlu(){
	mallocVL();
	line();
	yylval.valor_lexico->type = LITERAL;
	yylval.valor_lexico->litType = FLOAT;
	char* number = strdup(yytext);
	yylval.valor_lexico->value.flutuante = atof(number); 
}

void createLitChar(){
	mallocVL();
	line();
	yylval.valor_lexico->type = LITERAL;
	yylval.valor_lexico->litType = CHAR;
	char* c = strdup(yytext);
	yylval.valor_lexico->value.car = c[1]; // sem as aspas
}

void createBool(){
	mallocVL();
	line();
	yylval.valor_lexico->type = LITERAL;
	yylval.valor_lexico->litType = BOOL;
	yylval.valor_lexico->value.boolean = strdup(yytext);
}

void createLitString(){
	mallocVL();
	line();
	yylval.valor_lexico->type = LITERAL;
	yylval.valor_lexico->litType = STRING;
	char* string = strdup(yytext);
	string++; //remove first "
	for(int i = 0; i ++; ){
		if(string[i] == '"'){
			string[i] = '\0';
			break;
		}
	}
	yylval.valor_lexico->value.string = string;
	

}