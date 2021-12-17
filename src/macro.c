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

int exec_macro(key_macro *macro)
{
	if (curbp->modes & macro->modes) {
		macro->func(1,1);
		return true;
	}
	return false;
}

/*
 * change a key command to a string we can print out
 *
 * int c;		sequence to translate
 * char *seq;		destination string for sequence
 */
void cmdstr(int c, char *seq)
{
	char *ptr;		/* pointer into current position in sequence */

	ptr = seq;

	/* apply meta sequence if needed */
	if (c & META) {
		*ptr++ = 'M';
		*ptr++ = '-';
	}

	/* apply ^X sequence if needed */
	if (c & CTLX) {
		*ptr++ = '^';
		*ptr++ = 'X';
	}

	/* apply SPEC sequence if needed */
	if (c & SPEC) {
		*ptr++ = 'F';
		*ptr++ = 'N';
	}

	/* apply control sequence if needed */
	if (c & CONTROL) {
		*ptr++ = '^';
	}

	/* and output the final sequence */

	*ptr++ = c & 255;	/* strip the prefixes */

	*ptr = 0;		/* terminate the string */
}
