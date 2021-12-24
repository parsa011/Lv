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
 *	append given window into last windows next
 */
int append_window(window *wp)
{
	swnext(lastwp,wp);
	swprev(wp,lastwp);
	lastwp = curwp = wp; 
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

int next_window(int f, int n) 
{
	if (wnext(curwp) == NULL) {
		showmsg(true,"Last window");
		return false;
	}
	curwp = wnext(curwp);
	curbp = curwp->fbuffer;
	write_buffer();
	return true;
}

int prev_window(int f, int n) 
{
	if (wprev(curwp) == NULL) {
		showmsg(true,"Firt window");
		return false;
	}
	curwp = wprev(curwp);
	curbp = curwp->fbuffer;
	write_buffer();
	return true;
}
