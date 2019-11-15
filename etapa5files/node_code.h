#pragma once

enum {
	INVALID_CODE = 0,
	GVD,  // global var decl.
	GVECD,  // global vector decl.
	LVD,  // local var decl.
	LVDI,  // local var decl. com init.
	ASSIGN,  // assignment.
	LIT_VEC_IDX, // '[index]' para arrays multidimensionais. index eh um literal inteiro. --> Usado na declaração de vetores globais!
	EXP_VEC_IDX, // '[index]' para arrays multidimensionais. index pode ser expressao inteira. --> Usado ao fazer assigment com vetor!
	IF, // if.
	IF_ELSE, // if-else.
	WHILE, // while.
	FOR, // for.
	FLSC, // for list simple command.

	// operadores:
	OR,  // OK
	AND, // OK
	ADD, // OK
	SUB, // OK
	MULT, // OK
	DIV, // OK

	// operadores relacionais:
	LE,  // OK // <=
	GE,  // OK // >=
	EQ,  // OK // ==
	NEQ, // OK // !=
	LESS, // OK // <
	GREAT, // OK // >

	// expressoes literais:
	LITVAL,  // OK
	IDENT,
	EXPVEC,  // acesso a elemento de vetor como literal expression. index pode ser expressao inteira. -> Usado no acesso a vetor!
	EXPVEC_IDX_2 // '[index]' para arrays multidimensionais. -> Usado no acesso a vetor!

};

