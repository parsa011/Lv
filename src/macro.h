#ifndef _MACRO_H
# define _MACRO_H

/*
 *	Keyboard macros	
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define MMACRON 32	/* max length for macro name */

/*
 *	each macro should be stored in this struct
 */
struct key_macro_t {
	LINK(key_macro) link;
	int key;				/* key for macro						  */
	char name[MMACRON];		/* just a name for this macro			  */
	ushort modes;			/* in which modes this macro is available */
	void (*func)();			/* function to run when this macro called */
};

extern int macros_count;	/* total count of macros	 */
extern key_macro *fmacro;	/* first generated key macro */
extern key_macro *lmacro;	/* last appended key macro	 */

key_macro *init_macro(int , void (*)(), ushort, char *);

/*
 *	this function will initilize and append all of basic macros
 */
int generate_basic_macros();

/*
 *	add a key macro to last macros next
 */
void append_macro(key_macro *);

/*
 *	this routin will find a macro by it's keu
 */
key_macro *find_macro(int);

/*
 * find macro by it's name
 */
key_macro *find_macro_by_name(char *);

#endif
