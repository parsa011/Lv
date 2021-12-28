/*
 *	Keyboard macros	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

int macros_count = 0;
key_macro *fmacro;	/* first generated key macro */
key_macro *lmacro;	/* last appended key macro	 */

key_macro *init_macro(int key,char *key_str,MACRO_FUNC_POINTER func, ushort modes, char *name)
{
	key_macro *macro = calloc(1,sizeof(key_macro));
	macro->key = key;
	macro->func = func;
	macro->modes = modes;
	macro->key_str = strdup(key_str);
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
		if (macro->key == key && macro->modes & curbp->modes)
			return macro;
	return NULL;
}

key_macro *find_macro_str(char *str)
{
	for (key_macro *macro = fmacro; macro != NULL; macro = mnext(macro))
		if (strcmp(macro->key_str,str) == 0)
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
		if (number_stack == 0)
			number_stack = 1;
		macro->func(1,number_stack);
		number_stack = 0;
		return true;
	}
	return false;
}

/*
 *	will if we have macro to start with this name or 
 *	this needed we add key to macro stack , to check
 *	if we have a macro to start with stack or no , if
 *	we dont have anything like that , we will clear 
 *	stack
 */
bool compare_macro_name(char *name,int len)
{
	for (key_macro *macro = fmacro; macro != NULL; macro = mnext(macro))
		if (strncmp(macro->key_str,name,len) == 0)
			return true;
	return false;
}
