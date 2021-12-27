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

typedef int	(*MACRO_FUNC_POINTER)(int, int);	/* generally useful type */

/*
 *	each macro should be stored in this struct
 */
struct key_macro_t {
	LINK(key_macro) link;
	int key;				/* key for macro						  */
	char *key_str;			/* key as stering 						  */
	char name[MMACRON];		/* just a name for this macro			  */
	ushort modes;			/* in which modes this macro is available */
	MACRO_FUNC_POINTER func;	/* function to run when this macro called */
};

#define mnext(m) ((m)->link.next)
#define mprev(m) ((m)->link.prev)

extern int macros_count;	/* total count of macros	 */
extern key_macro *fmacro;	/* first generated key macro */
extern key_macro *lmacro;	/* last appended key macro	 */

key_macro *init_macro(int,char *,MACRO_FUNC_POINTER,ushort,char *);

/*
 *	add a key macro to last macros next
 */
void append_macro(key_macro *);

/*
 *	this routin will find a macro by it's key
 */
key_macro *find_macro(int);
key_macro *find_macro_str(char *);
/*
 * find macro by it's name
 */
key_macro *find_macro_by_name(char *);

/*
 *	execute a macro , check for modes
 */
int exec_macro(key_macro *);

#endif
