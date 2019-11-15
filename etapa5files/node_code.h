#pragma once

enum {
	INVALID_CODE = 0,
	GVD,  // global var decl.
	LVD,  // local var decl.
	LVDI,  // local var decl. com init.
	ASSIGN,  // assignment.
	LIT_VEC_IDX, // '[index]' para arrays multidimensionais. index eh um literal inteiro.
	EXP_VEC_IDX, // '[index]' para arrays multidimensionais. index pode ser expressao inteira.
};

