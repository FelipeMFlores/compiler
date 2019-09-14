
%{
#include <stdio.h>
int yylex(void);
void yyerror (char const *s);
extern int get_line_number();
int erro = 0;
%}
%define parse.error verbose
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_CHAR
%token TK_PR_STRING
%token TK_PR_IF
%token TK_PR_THEN
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_DO
%token TK_PR_INPUT
%token TK_PR_OUTPUT
%token TK_PR_RETURN
%token TK_PR_CONST
%token TK_PR_STATIC
%token TK_PR_FOREACH
%token TK_PR_FOR
%token TK_PR_SWITCH
%token TK_PR_CASE
%token TK_PR_BREAK
%token TK_PR_CONTINUE
%token TK_PR_CLASS
%token TK_PR_PRIVATE
%token TK_PR_PUBLIC
%token TK_PR_PROTECTED
%token TK_PR_END
%token TK_PR_DEFAULT
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_OC_SL
%token TK_OC_SR
%token TK_OC_FORWARD_PIPE
%token TK_OC_BASH_PIPE
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_LIT_CHAR
%token TK_LIT_STRING
%token TK_IDENTIFICADOR
%token TOKEN_ERRO
%%

program:			decl_var_glob program {if(erro) YYABORT;} 
					| func program { if(erro) YYABORT;} 
					| %empty { if(erro) YYABORT;} 
					| error program {erro = 1;}
;

decl_var_glob:		TK_PR_STATIC type TK_IDENTIFICADOR ';' |
					type TK_IDENTIFICADOR ';' |
					TK_PR_STATIC type TK_IDENTIFICADOR '[' TK_LIT_INT ']' ';' |
					type TK_IDENTIFICADOR '[' TK_LIT_INT ']' ';'
;

func:				TK_PR_STATIC type TK_IDENTIFICADOR param_list command_block |
					type TK_IDENTIFICADOR param_list command_block
;

param_list:			'(' ')' |
					'(' param_list_aux
;

param_list_aux:		type TK_IDENTIFICADOR ')' |
					type TK_IDENTIFICADOR ',' param_list_aux |
					TK_PR_CONST type TK_IDENTIFICADOR ')' |
					TK_PR_CONST type TK_IDENTIFICADOR ',' param_list_aux
;

command_block:		'{' '}' |
					'{' command_block_aux
;

command_block_aux:	simple_command ';' '}' |
					simple_command ';' command_block_aux |
					control_flow_command '}' |
					control_flow_command command_block_aux
					| error '}' {erro = 1;}
					| error ';' command_block_aux {erro = 1;}
;

simple_command:		command_block | decl_var_local | assignment | input | output |
					func_call | shift | return | break | continue
;

decl_var_local:		local_var_qualifier type TK_IDENTIFICADOR |
					type TK_IDENTIFICADOR |
					local_var_qualifier type TK_IDENTIFICADOR local_var_init |
					type TK_IDENTIFICADOR local_var_init
;

local_var_qualifier:	TK_PR_STATIC | TK_PR_CONST | TK_PR_STATIC TK_PR_CONST
;

local_var_init:		TK_OC_LE TK_IDENTIFICADOR |
					TK_OC_LE litValue
;

litValue:			TK_LIT_INT | TK_LIT_FLOAT | TK_LIT_FALSE | TK_LIT_TRUE | TK_LIT_CHAR | TK_LIT_STRING
;

assignment:			TK_IDENTIFICADOR '=' expression |
					TK_IDENTIFICADOR '[' expression ']' '=' expression
;

input:				TK_PR_INPUT expression
;

output:				TK_PR_OUTPUT out_expr_list
;

out_expr_list:		expression | expression ',' out_expr_list
;

func_call:			TK_IDENTIFICADOR '(' ')' |
					TK_IDENTIFICADOR '(' func_call_list
;

func_call_list:		expression ')' |
					expression ',' func_call_list
;

shift:				TK_IDENTIFICADOR TK_OC_SR expression |
					TK_IDENTIFICADOR TK_OC_SL expression |
					TK_IDENTIFICADOR '[' expression ']' TK_OC_SR expression |
					TK_IDENTIFICADOR '[' expression ']' TK_OC_SL expression
;

return:				TK_PR_RETURN expression
;

break:				TK_PR_BREAK
;

continue:			TK_PR_CONTINUE
;

control_flow_command:	TK_PR_IF '(' expression ')' command_block |
						TK_PR_IF '(' expression ')' command_block TK_PR_ELSE command_block |
						TK_PR_WHILE '(' expression ')' TK_PR_DO command_block |
						for_command
;

for_command:		TK_PR_FOR '(' for_list ':' expression ':' for_list ')' command_block
;

for_list:			for_list_simple_command |
					for_list_simple_command ',' for_list
;

for_list_simple_command:	
					command_block |
					decl_var_local |
					assignment |
					input |
					shift |
					return |
					break |
					continue
;

type:				TK_PR_INT | TK_PR_FLOAT | TK_PR_BOOL | TK_PR_CHAR | TK_PR_STRING
;


expression:			l11 ;
l11:				l10 '?' l10 ':' l10 | l10 ;
l10:				l10 relational_operator l9 | l9 ;
l9:					l9 TK_OC_OR l8 | l8 ;
l8: 				l8 TK_OC_AND l7 | l7 ;
l7:					l7 TK_OC_FORWARD_PIPE l6 | l7 TK_OC_BASH_PIPE l6 | l6 ;
l6:					l6 TK_OC_SL l5 | l6 TK_OC_SR l5 | l5 ;
l5:					l5 '+' l4 | l5 '-' l4 | l4 ;
l4:					l4 '&' l3 | l4 '|' l3 | l3 ;
l3:					l3 '*' l2 | l3 '/' l2 | l3 '%' l2 | l2 ;
l2:					l2 '^' l1 | l1 ;
l1:					unary_operator l1  | l0 ;
l0:					literal_expression | '(' expression ')' ;

relational_operator:	TK_OC_LE | TK_OC_GE | TK_OC_EQ | TK_OC_NE | '<' | '>' ;

unary_operator:			'+' | '-' | '!' | '&' | '*' | '?' | '#' ;

literal_expression:		
					litValue | 
					TK_IDENTIFICADOR | 
					TK_IDENTIFICADOR '[' expression ']' |
					func_call
;

%%

void yyerror (char const *s){
	int line = get_line_number();
    fprintf (stderr, "%s , line: %d \n", s, line);
}