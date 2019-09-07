%{
#include "parser.tab.h"

int line_counter = 1;

%}

letra [a-z]|[A-Z]|_
numero [0-9]
id {letra}({letra}|{numero})*
especialChar ["," ";" ":" "(" ")" \] \[ "{" "}" "+" \- "|" "?" "@" "*" "/" "<" ">" "=" "!" "&" "%" "#" "^" "." "$" "~" "`"]

litInt ("+"|"-")?{numero}+

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
int						{ return TK_PR_INT          ; }
float					{ return TK_PR_FLOAT        ; }  
bool					{ return TK_PR_BOOL         ; }  
char					{ return TK_PR_CHAR         ; }  
string					{ return TK_PR_STRING       ; }  
if						{ return TK_PR_IF           ; }  
then					{ return TK_PR_THEN         ; } 
else					{ return TK_PR_ELSE         ; }  
while					{ return TK_PR_WHILE        ; }  
do						{ return TK_PR_DO           ; }  
input					{ return TK_PR_INPUT        ; }  
output					{ return TK_PR_OUTPUT       ; }  
return					{ return TK_PR_RETURN       ; }  
const					{ return TK_PR_CONST        ; }  
static					{ return TK_PR_STATIC       ; }  
foreach					{ return TK_PR_FOREACH      ; }  
for						{ return TK_PR_FOR          ; } 
switch					{ return TK_PR_SWITCH       ; }  
case					{ return TK_PR_CASE         ; }  
break					{ return TK_PR_BREAK        ; }  
continue				{ return TK_PR_CONTINUE     ; }  
class					{ return TK_PR_CLASS        ; }  
private					{ return TK_PR_PRIVATE      ; }  
public					{ return TK_PR_PUBLIC       ; }  
protected				{ return TK_PR_PROTECTED    ; }  

false					{ return TK_LIT_FALSE		; }
true 					{ return TK_LIT_TRUE        ; }

"<="					{ return TK_OC_LE           ; }  
">="					{ return TK_OC_GE           ; } 
"=="					{ return TK_OC_EQ           ; }  
"!="					{ return TK_OC_NE           ; }  
"&&"					{ return TK_OC_AND          ; }  
"||"					{ return TK_OC_OR           ; }  
">>"					{ return TK_OC_SL           ; }  
"<<"					{ return TK_OC_SR           ; }  
"%>%"					{ return TK_OC_FORWARD_PIPE ; }  
"%|%"					{ return TK_OC_BASH_PIPE    ; }  
end						{ return TK_PR_END          ; } 
default					{ return TK_PR_DEFAULT      ; }
{id}					{ return TK_IDENTIFICADOR	; }
{especialChar}			{ return yytext[0]			; }

{litInt}				{ return TK_LIT_INT			; }
{litFloat}				{ return TK_LIT_FLOAT		; }
{litChar}				{ return TK_LIT_CHAR		; }
{litString}				{ return TK_LIT_STRING		; }

"\n"					line_counter++;


({litInt}|{litFloat})([^"+""-""*""/""%""<"">""<="">=""==""!=""&&""||"">>""<<""%>%""%|%""|""&""^"";"" ""\n""\t"])*	{ return TOKEN_ERRO	; }


\'[^(\'|{letra}|{numero}|{especialChar})]*\'	{ return TOKEN_ERRO ;}
\'{id}\'				{ return TOKEN_ERRO 		; }

.						{ return TOKEN_ERRO			; }

%%


int get_line_number () {
	return line_counter;
}

