#pragma once

enum {
	INVALID_CODE = 0,
	GVD,  // global var decl.
	LVD,  // local var decl.
	LVDI,  // local var decl. com init.
	ASSIGN,  // assignment.
	VECIDX, // '[index]' para arrays multidimensionais.
};

