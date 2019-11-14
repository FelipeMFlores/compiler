
%{
#include <stdio.h>
#include "etapa4files/hashtable.h"
#include "etapa4files/auxiliary.h"
#include "etapa5files/node_code.h"
#include "etapa5files/codeGenerator.h"

int yylex(void);
extern void* arvore;
void yyerror (char const *s);
extern int get_line_number();
int erro = 0;

// declara curr_hashtable e inicializa com NULL.
HASHTABLE *curr_hashtable = NULL;

// inicializa curr_hashtable se ainda nao foi inicializada.
#define TCH if(!curr_hashtable){curr_hashtable=create_hashtable();}

// identifica se o bloco de comandos a ser criado e de uma funcao:
int commandblock_from_function = 0;
valor_lexico *return_type = NULL;

%}
%union {
	struct node* node;
	struct lexico *valor_lexico;
	struct valor_lexico_list *valor_lexico_list;
}

%define parse.error verbose
%token <valor_lexico> TK_PR_INT
%token <valor_lexico> TK_PR_FLOAT
%token <valor_lexico> TK_PR_BOOL
%token <valor_lexico> TK_PR_CHAR
%token <valor_lexico> TK_PR_STRING
%token <valor_lexico> TK_PR_IF
%token <valor_lexico> TK_PR_THEN
%token <valor_lexico>  TK_PR_ELSE
%token <valor_lexico>  TK_PR_WHILE
%token <valor_lexico>  TK_PR_DO
%token <valor_lexico>  TK_PR_INPUT
%token <valor_lexico>  TK_PR_OUTPUT
%token <valor_lexico>  TK_PR_RETURN
%token <valor_lexico>  TK_PR_CONST
%token <valor_lexico>  TK_PR_STATIC
%token <valor_lexico>  TK_PR_FOREACH
%token <valor_lexico>  TK_PR_FOR
%token <valor_lexico>  TK_PR_SWITCH
%token <valor_lexico>  TK_PR_CASE
%token <valor_lexico>  TK_PR_BREAK
%token <valor_lexico>  TK_PR_CONTINUE
%token <valor_lexico>  TK_PR_CLASS
%token <valor_lexico>  TK_PR_PRIVATE
%token <valor_lexico>  TK_PR_PUBLIC
%token <valor_lexico>  TK_PR_PROTECTED
%token <valor_lexico>  TK_PR_END
%token <valor_lexico>  TK_PR_DEFAULT
%token <valor_lexico>  TK_OC_LE
%token <valor_lexico>  TK_OC_GE
%token <valor_lexico>  TK_OC_EQ
%token <valor_lexico>  TK_OC_NE
%token <valor_lexico>  TK_OC_AND
%token <valor_lexico>  TK_OC_OR
%token <valor_lexico>  TK_OC_SL
%token <valor_lexico>  TK_OC_SR
%token <valor_lexico>  TK_OC_FORWARD_PIPE
%token <valor_lexico>  TK_OC_BASH_PIPE
%token <valor_lexico>  TK_LIT_INT
%token <valor_lexico>  TK_LIT_FLOAT
%token <valor_lexico>  TK_LIT_FALSE
%token <valor_lexico>  TK_LIT_TRUE
%token <valor_lexico>  TK_LIT_CHAR
%token <valor_lexico>  TK_LIT_STRING
%token <valor_lexico>  TK_IDENTIFICADOR
%token <valor_lexico>  TOKEN_ERRO
%token <valor_lexico> ','  ';'   ':'   '('   ')'   '['   ']'  '{'   '}'  '+'   '-'   '|'   '?'   '@' '*'   '/'   '<'   '>'   '=' '!'   '&'   '%'   '#'   '^'   '.'   '$'   '~'   '`'


%type <node> program
%type <node> func
%type <node> command_block
%type <node> command_block_aux
%type <node> simple_command
%type <node> decl_var_local
%type <node> local_var_init
%type <node> litValue
%type <node> assignment
%type <node> func_call
%type <node> func_call_list
%type <node> shift
%type <node> return
%type <node> break
%type <node> continue
%type <node> control_flow_command
%type <node> for_command
%type <node> for_list
%type <node> for_list_simple_command
%type <node> expression
%type <node> l0 l1 l2 l3 l4 l5 l6 l7 l8 l9 l10 l11
%type <node> relational_operator
%type <node> unary_operator
%type <node> literal_expression
%type <node> assignment_vector
%type <node> expression_vector

%type <node> decl_var_glob

%type <valor_lexico> type


%type <valor_lexico_list> param_list;
%type <valor_lexico_list> param_list_aux;

%%

program:			decl_var_glob program {if(erro) YYABORT; $$ = $2;} 
					| func program { if(erro) YYABORT; $$ = $1; arvore = $$; addChild($$, $2);}  //uma funcao filha da outra
					| %empty { if(erro) YYABORT; $$ = NULL;} 
					| error program {YYABORT;}
;

decl_var_glob:		TK_PR_STATIC type TK_IDENTIFICADOR ';' {TCH; $$ = newNode($3); setCode($$, GVD); set_type_by_vl($$, $2); insert_var_decl(curr_hashtable, $2, $3); } |
					type TK_IDENTIFICADOR ';' {TCH; $$ = newNode($2); setCode($$, GVD); set_type_by_vl($$, $1); insert_var_decl(curr_hashtable, $1, $2); } |
					TK_PR_STATIC type TK_IDENTIFICADOR vector ';' {TCH $$ = newNode($3); setCode($$, GVD); set_type_by_vl($$, $2); addChild($$, $4); insert_vec_decl(curr_hashtable, $2, $3); } |
					type TK_IDENTIFICADOR vector';' {TCH $$ = newNode($2); setCode($$, GVD); set_type_by_vl($$, $1); addChild($$, $3); insert_vec_decl(curr_hashtable, $1, $2); }
;

vector: '[' TK_LIT_INT ']' vector {TCH $$ = newNode($2); addChild($$, $4); }
		| '[' TK_LIT_INT ']' {TCH $$ = newNode($2);}
;

func:				TK_PR_STATIC type TK_IDENTIFICADOR param_list {commandblock_from_function = 1; return_type = $2;} command_block {TCH $$ = newNode($3); addChild($$, $6); 
						insert_func_decl(curr_hashtable, $2, $3, $4);} |
					type TK_IDENTIFICADOR param_list {commandblock_from_function = 1; return_type = $1;} command_block {TCH $$ = newNode($2); addChild($$, $5); 
						insert_func_decl(curr_hashtable, $1, $2, $3);}
;

param_list:			'(' ')' {$$ = NULL;} |
					'(' param_list_aux {$$ = $2;}
;

param_list_aux:		type TK_IDENTIFICADOR ')' {TCH $$ = newValorLexicoList($1); $$->prox = NULL;} |
					type TK_IDENTIFICADOR ',' param_list_aux {TCH $$ = newValorLexicoList($1); $$->prox = $4;} |
					TK_PR_CONST type TK_IDENTIFICADOR ')' {TCH $$ = newValorLexicoList($2); $$->prox = NULL;} |
					TK_PR_CONST type TK_IDENTIFICADOR ',' param_list_aux {TCH $$ = newValorLexicoList($2); $$->prox = $5;}
;

command_block:		'{' '}' {$$ = NULL; commandblock_from_function = 0; }
					| '{' 
							{TCH HASHTABLE *aux = create_hashtable(); aux->prev = curr_hashtable; curr_hashtable = aux;   // cria novo escopo.
									if (commandblock_from_function){aux->return_type = type_from_vl(return_type);} commandblock_from_function = 0; }
								command_block_aux 
							{$$ = $3;} 
						
;

command_block_aux:	simple_command ';' '}' {TCH $$ = $1; curr_hashtable = curr_hashtable->prev; } |   // fecha escopo atual.
					simple_command ';' command_block_aux { if($1 != NULL){$$ = $1; addChild($$, $3);} //um simple é filho do outro
															else $$ = $3; } // primeiro simple pode ser ignorado na arvore
					| control_flow_command ';' '}' {TCH $$ = $1; curr_hashtable = curr_hashtable->prev; } |
					control_flow_command ';' command_block_aux {$$ = $1; addChild($$, $3);}
					| error '}' {TCH erro = 1; curr_hashtable = curr_hashtable->prev; }
					| error ';' command_block_aux {erro = 1;}
;

simple_command:		command_block {$$ = NULL;}
					| decl_var_local {$$ = $1;}
					| assignment {$$ = $1;}
					| input {$$ = NULL;}
					| output {$$ = NULL;}
					| func_call {$$ = $1;}
					| shift {$$ = $1;}
					| return {$$ = $1;}
					| break {$$ = $1;}
					| continue {$$ = $1;}
;

decl_var_local:		local_var_qualifier type TK_IDENTIFICADOR {TCH $$ = NULL; insert_var_decl(curr_hashtable, $2, $3); }
					| type TK_IDENTIFICADOR {TCH $$ = NULL; insert_var_decl(curr_hashtable, $1, $2); }
					| local_var_qualifier type TK_IDENTIFICADOR local_var_init {TCH $$ = $4; addChild($$, newNode($3)); //pai é o <= 
									insert_var_decl(curr_hashtable, $2, $3); assert_compatible_type_local_var_init(curr_hashtable, $2, $4); }
					| type TK_IDENTIFICADOR local_var_init {TCH $$ = $3; addChild($$, newNode($2));
									insert_var_decl(curr_hashtable, $1, $2); assert_compatible_type_local_var_init(curr_hashtable, $1, $3); }

;

local_var_qualifier:	TK_PR_STATIC | TK_PR_CONST | TK_PR_STATIC TK_PR_CONST
;

local_var_init:		TK_OC_LE TK_IDENTIFICADOR {TCH $$ = newNode($1); addChild($$, newNode($2)); assert_var_exists(curr_hashtable, $2);}
					| TK_OC_LE litValue {$$ = newNode($1); addChild($$, $2);}
;

litValue:			TK_LIT_INT {$$ = newNode($1);}
					| TK_LIT_FLOAT {$$ = newNode($1);}
					| TK_LIT_FALSE {$$ = newNode($1);}
					| TK_LIT_TRUE {$$ = newNode($1);}
					| TK_LIT_CHAR {$$ = newNode($1);}
					| TK_LIT_STRING {$$ = newNode($1);}
;

assignment:			TK_IDENTIFICADOR '=' expression {TCH $$ = newNode($2); addChild($$, newNode($1)); addChild($$, $3);
														assert_var_exists(curr_hashtable, $1);
														assert_compatible_type_assignment(curr_hashtable, $1, $3);
														} //pai é o =
					| TK_IDENTIFICADOR assignment_vector '=' expression {TCH $$ = newNode($3); 
																			NODE* i = newNode($1);
																			addChild(i, $2);
																			addChild($$, i); addChild($$, $4);
																			assert_vec_exists(curr_hashtable, $1);
																			assert_compatible_type_assignment(curr_hashtable, $1, $4);
																			}
;

assignment_vector: '[' expression ']' assignment_vector {$$ = $2; addChild($$, $4); assert_integer_expression($2);}
					| '[' expression ']' {$$ = $2; assert_integer_expression($2);}
;

input:				TK_PR_INPUT expression  {assert_input_param_is_identifier($2);}
;

output:				TK_PR_OUTPUT out_expr_list
;

out_expr_list:		expression | expression ',' out_expr_list
;


func_call:			TK_IDENTIFICADOR '(' ')' {TCH $$ = newNode($1); assert_func_exists(curr_hashtable, $1, NULL); 
												set_type_from_identifier_in_hashtable(curr_hashtable, $$, $1);}
					| TK_IDENTIFICADOR '(' func_call_list {TCH $$ = newNode($1); addChild($$, $3); assert_func_exists(curr_hashtable, $1, $3); 
												set_type_from_identifier_in_hashtable(curr_hashtable, $$, $1);}
;

func_call_list:		expression ')' {$$ = $1; $$->is_another_argument = 1; }
					| expression ',' func_call_list {$$ = $1; $$->is_another_argument = 1; addChild($$, $3);}
;

shift:				TK_IDENTIFICADOR TK_OC_SR expression {TCH $$ = newNode($2); addChild($$, newNode($1)); addChild($$, $3); 
															assert_var_exists(curr_hashtable, $1);}
					| TK_IDENTIFICADOR TK_OC_SL expression {TCH $$ = newNode($2); addChild($$, newNode($1)); addChild($$, $3);
															assert_var_exists(curr_hashtable, $1);}
					| TK_IDENTIFICADOR '[' expression ']' TK_OC_SR expression {TCH $$ = newNode($5); 
																			NODE* i = newNode($1);
																			addChild(i, $3);
																			addChild($$, i); addChild($$, $6);
																			assert_vec_exists(curr_hashtable, $1);
																			}
					| TK_IDENTIFICADOR '[' expression ']' TK_OC_SL expression {TCH $$ = newNode($5); 
																			NODE* i = newNode($1);
																			addChild(i, $3);
																			addChild($$, i); addChild($$, $6);
																			assert_vec_exists(curr_hashtable, $1);
																			}
;

return:				TK_PR_RETURN expression {$$ = newNode($1); addChild($$, $2); assert_compatible_return_type(curr_hashtable, $2, 															get_line_number()); }
;

break:				TK_PR_BREAK {$$ = newNode($1);}
;

continue:			TK_PR_CONTINUE {$$ = newNode($1);}
;

control_flow_command:
		TK_PR_IF '(' expression ')' command_block {$$ = newNode($1); addChild($$, $3); addChild($$, $5);} |
		TK_PR_IF '(' expression ')' command_block TK_PR_ELSE command_block {$$ = newNode($1); addChild($$, $3); addChild($$, $5); addChild($$, $7);} |
		TK_PR_WHILE '(' expression ')' TK_PR_DO command_block {$$ = newNode($1); addChild($$, $3); addChild($$, $6);} |
		for_command {$$ = $1;}
;

for_command:		
		TK_PR_FOR '(' for_list ':' expression ':' for_list ')' command_block {$$ = newNode($1); addChild($$, $3); addChild($$, $5); addChild($$, $7); addChild($$, $9);}
;

for_list:
		for_list_simple_command {$$ = $1;} |
		for_list_simple_command ',' for_list {$$ = $1; addChild($$, $3);}
;

for_list_simple_command:
					command_block {$$ = $1;} |
					decl_var_local {$$ = $1;} |
					assignment {$$ = $1;} |
					input {$$ = NULL;}|
					shift {$$ = $1;} |
					return {$$ = $1;} |
					break {$$ = $1;} |
					continue {$$ = $1;}
;

type:				TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING
;


expression:			l11 {$$ = $1;}
;
l11:				l10 '?' l10 ':' l10 {$$ = newNode($4); addChild($$, $1); addChild($$, $3); addChild($$, $5);
											assert_int_float_or_bool($1); infer_type_binop($$, $3, $5); }
					| l10 {$$ = $1;}
;
l10:				l10 relational_operator l9 {$$ = $2; addChild($$, $1); addChild($$, $3);
											assert_int_float_or_bool($1); assert_int_float_or_bool($3);
											set_type($$, TIPO_BOOL); }
					| l9 {$$ = $1;}
;
l9:					l9 TK_OC_OR l8 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
											assert_int_float_or_bool($1); assert_int_float_or_bool($3);
											set_type($$, TIPO_BOOL); }
					| l8  {$$ = $1;}
;
l8: 				l8 TK_OC_AND l7 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
											assert_int_float_or_bool($1); assert_int_float_or_bool($3);
											set_type($$, TIPO_BOOL); }
					| l7  {$$ = $1;}
;
l7:					l7 TK_OC_FORWARD_PIPE l6 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
											assert_int_float_or_bool($1); assert_int_float_or_bool($3);
											set_type($$, TIPO_INT); }
					| l7 TK_OC_BASH_PIPE l6 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
											assert_int_float_or_bool($1); assert_int_float_or_bool($3);
											set_type($$, TIPO_INT); }
					| l6  {$$ = $1;}
;
l6:					l6 TK_OC_SL l5 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
									assert_int_float_or_bool($1); assert_int_float_or_bool($3);
									set_type($$, TIPO_INT);}
					| l6 TK_OC_SR l5 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
									assert_int_float_or_bool($1); assert_int_float_or_bool($3);
									set_type($$, TIPO_INT);}
					| l5  {$$ = $1;}
;
l5:					l5 '+' l4 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}
					| l5 '-' l4 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}
					| l4  {$$ = $1;}
;
l4:					l4 '&' l3 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}
					| l4 '|' l3 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}
					| l3  {$$ = $1;}
;
l3:					l3 '*' l2 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}

					| l3 '/' l2 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}

					| l3 '%' l2 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}

					| l2  {$$ = $1;}
;
l2:					l2 '^' l1 {$$ = newNode($2); addChild($$, $1); addChild($$, $3);
								assert_int_float_or_bool($1); assert_int_float_or_bool($3);
								infer_type_binop($$, $1, $3);}

					| l1  {$$ = $1;}
;
l1:					unary_operator l1  {$$ = $1; addChild($$, $2);
								copy_type($2, $$);}
					| l0 {$$ = $1;}
;
l0:					literal_expression {$$ = $1;}
					| '(' expression ')' {$$ = $2;}
					;

relational_operator:	TK_OC_LE {$$ = newNode($1);} 
						|	TK_OC_GE {$$ = newNode($1);} 
						|	TK_OC_EQ {$$ = newNode($1);} 
						|	TK_OC_NE {$$ = newNode($1);} 
						|	'<'		 {$$ = newNode($1);} 
						|	'>'		 {$$ = newNode($1);} 
						;

unary_operator:			'+'	{$$ = newNode($1);}	
						|	'-'	{$$ = newNode($1);}	
						|	'!'	{$$ = newNode($1);}	
						|	'&'	{$$ = newNode($1);}	
						|	'*'	{$$ = newNode($1);}	
						|	'?'	{$$ = newNode($1);}	
						|	'#' {$$ = newNode($1);}
						;


literal_expression:		
					litValue {$$ = $1; set_type_by_vl($$, ($1)->data);}  
					| TK_IDENTIFICADOR {TCH $$ = newNode($1); 
						assert_var_exists(curr_hashtable, $1); set_type_from_identifier_in_hashtable(curr_hashtable, $$, $1); } 
					| TK_IDENTIFICADOR expression_vector {TCH $$ = newNode($1); addChild($$, $2);
						assert_vec_exists(curr_hashtable, $1);  set_type_from_identifier_in_hashtable(curr_hashtable, $$, $1); } 
					| func_call {$$ = $1; copy_type($1, $$);}
;

expression_vector:  '[' expression ']' expression_vector {$$ = $2; addChild($$, $4); assert_integer_expression($2);}
					| '[' expression ']' {$$ = $2; assert_integer_expression($2);}
;

%%

void yyerror (char const *s){
	int line = get_line_number();
    fprintf (stdout, "%s , line: %d \n", s, line);
}