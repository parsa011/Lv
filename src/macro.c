#include "types.h"

/*
 *	Keyboard macros	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

int macros_count = 0
key_macro *fmacro;	/* first generated key macro */
key_macro *lmacro;	/* last appended key macro	 */

key_macro *init_macro(int key, void (*func)(), ushort modes, char *name)
{
	key_macro *macro = calloc(1,sizeof(key_macro));
	macro->key = key;
	macro->func = func;
	macro->modes = mdoes;
	memcpy(macro->name,name,MMACRON);
	return macro;
}

void append_macro(key_macro *macro)
{
	if (fmacro == NULL)	{
		fmacro = macro;
	} else {
		lmacro->link.next = macro;
	}
	lmacro = macro;
	macros_count++;
}

int generate_basic_macros()
{
	append_macro(init_macro(CTRL_KEY('q'),close_editor,0,"close editor"));
	return macros_count;
}
