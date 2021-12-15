#include "types.h"

/*
 *	Keyboard macros	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

int macros_count = 0;
key_macro *fmacro;	/* first generated key macro */
key_macro *lmacro;	/* last appended key macro	 */

key_macro *init_macro(int key, int (*func)(int, int), ushort modes, char *name)
{
	key_macro *macro = calloc(1,sizeof(key_macro));
	macro->key = key;
	macro->func = func;
	macro->modes = modes;
	memcpy(macro->name,name,strlen(name));
	return macro;
}

void append_macro(key_macro *macro)
{
	if (fmacro == NULL)	{
		fmacro = macro;
	} else {
		lmacro->link.next = macro;
	}
	macro->link.prev = lmacro;
	lmacro = macro;
	macros_count++;
}

key_macro *find_macro(int key)
{
	for (key_macro *macro = fmacro; macro != NULL; macro = mnext(macro))
		if (macro->key == key)
			return macro;
	return NULL;
}

key_macro *find_macro_by_name(char *name)
{
	for (key_macro *macro = fmacro; macro != NULL; macro = mnext(macro))
		if (strcmp(macro->name,name) == 0)
			return macro;
	return NULL;
}

int generate_basic_macros()
{
	append_macro(init_macro(CTRL_KEY('q'),close_editor,0,"close editor"));

	append_macro(init_macro(CTRL_KEY('k'),move_nextline,0,"go to next line"));
	append_macro(init_macro(CTRL_KEY('j'),move_prevline,0,"go to prev line"));
	append_macro(init_macro(CTRL_KEY('l'),next_char,0,"go to next char"));
	append_macro(init_macro(CTRL_KEY('h'),prev_char,0,"go to prev char"));

	return macros_count;
}
