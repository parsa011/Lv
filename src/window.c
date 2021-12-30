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
	wp->crow = wp->ccol = 1;
	return wp;
}

/*
 *	remove a window and kill it's buffer 
 *	also it will check for all possible states , for example 
 *	check if given window is last , or first , check lastwp and ...
 */
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
		firstwp = nwp;
		activate_window(nwp);
		goto free_wp;
	}
	if (nwp == NULL) {
		swnext(pwp,NULL);
		lastwp = pwp;
		activate_window(pwp);
		goto free_wp;
	}
	swnext(pwp,nwp);
	swprev(nwp,pwp);
	activate_window(pwp);
	if (compare_windows(firstwp,wp)) {
	}
free_wp:
	//destory_buffer(wp);
	free(wp);
	return true;
}

/*
 *	it's very simple for check , we have to enhance it
 */
bool compare_windows(window *wp1,window *wp2)
{
	if (wp1->crow == wp2->crow &&
		wp1->ccol == wp2->ccol &&
		wp1->flags == wp2->flags &&
		wp1->cbindex == wp2->cbindex) 
		return true;
	return false;
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
buffer *get_last_buffer()
{
	buffer *b = curwp->fbuffer;
	for (; bnext(b) != NULL;b = bnext(b));
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

int window_vertinal_split(int f,int n)
{
	if (curbp->nrow < 5) {
		showmsg(true,"Cant split under 5 line buffer");
		return false;
	}
	// init new buffer
	buffer *bf = init_buffer("","",0,FREDRW);
	curbp->nrow = curbp->nrow / 2;
	bf->mtop = curbp->mtop + curbp->nrow;
	bf->nrow = curbp->nrow;
	curbp->flags |= FREDRW;
	append_buffer(bf);
	return true;
}
