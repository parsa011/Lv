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

int remove_window(window *wp)
{
	window *pwp = wprev(wp);
	window *nwp = wnext(wp);
	if (nwp == NULL && pwp == NULL) {
		free(wp);
		return ALONEWINDOW;
	}
	if (pwp == NULL) {
		swprev(nwp,NULL);
		activate_window(nwp);
		goto free_wp;
	}
	if (nwp == NULL) {
		swnext(pwp,NULL);
		activate_window(pwp);
		goto free_wp;
	}
	swnext(pwp,nwp);
	swprev(nwp,pwp);
	activate_window(pwp);

free_wp:
	//destory_buffer(wp);
	free(wp);
	return true;
}

/*
 *	set curwp to given wp
 *	and set its first buffer to curbp , and also set
 *	redraw flag for buffer to re-write it
 */
void activate_window(window *wp)
{
	curwp = wp;
	curbp = curwp->fbuffer;
	curbp->flags |= FREDRW;
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
	activate_window(wnext(curwp));
	return true;
}

int prev_window(int f, int n) 
{
	if (wprev(curwp) == NULL) {
		showmsg(true,"Firt window");
		return false;
	}
	activate_window(wprev(curwp));
	return true;
}
