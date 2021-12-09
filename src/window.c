/*
 *	Manager windows	(and linux hahah)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#include "types.h"

window *init_window()
{
	window *wp;
	if (!(wp = malloc(sizeof(window))))
		die("malloc window");
	return wp;
}

/*
 *	return last buffer of given window
 *	if window is NULL we will get last buffer of curwp
 */
buffer *get_last_buffer(window *wp)
{
	/* set to curwp if wp is null */
	window *win = wp != NULL ? wp : curwp;
	buffer *b = win->fbuffer;
	for (; b != NULL;b = bnext(b));
	return b;
}
