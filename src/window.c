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
	wp->cbindex = 0;
	wp->crow = wp->ccol = 1;
	wp->link.next = wp->link.prev = 0;
	wp->fbuffer = NULL;
	return wp;
}

/*
 *	remove a window and kill it's buffer 
 *	also it will check for all possible states , for example 
 *	check if given window is last , or first , check lastwp and ...
 */
//int remove_window(window *wp)
//{
//	if (wcount == 1) {
//		free(wp);
//		return ALONEWINDOW;
//	}
//	window *new_one = L_LINK_PREV(wp);
//	if (new_one == 0) {
//		new_one == L_LINK_NEXT(wp);
//	}
//	if (wp == lastwp)
//		lastwp = new_one;
//	if (wp == firstwp)
//		firstwp = new_one;
//	activate_window(new_one);
//	L_LINK_REMOVE(wp);
//	free(wp);
//	return true;
//}

/*
 *	set curwp to given wp
 *	and set its first buffer to curbp , and also set
 *	redraw flag for buffer to re-write it
 */
void activate_window(window *wp)
{
	curwp = wp;
	curbp = get_buffer_by_index(curwp,curwp->cbindex);
	curbp->flags |= FREDRW;
}

/*
 *	append given window into last windows next
 */
//int append_window(window *wp)
//{
//	if (firstwp == NULL) {
//		curwp = wp;
//		firstwp = curwp;
//		lastwp = curwp;
//	} else {
//		L_LINK_INSERT(lastwp,wp);
//	}
//	lastwp = curwp = wp;
//	wcount++;
//}

/*
 *	return last buffer of given window
 *	if window is NULL we will get last buffer of curwp
 */
buffer *get_last_buffer(window *win)
{
	buffer *b = win->fbuffer;
	for (; L_LINK_NEXT(b) != NULL;b = L_LINK_NEXT(b));
	return b;
}

/*
 *	set next window as active window
 */
int next_window(int f, int n) 
{
	if (L_LINK_NEXT(curwp) == NULL) {
		showmsg(true,"Last window");
		return false;
	}
	activate_window(L_LINK_NEXT(curwp));
	return true;
}

int prev_window(int f, int n)
{
	if (L_LINK_PREV(curwp) == NULL) {
		showmsg(true,"Firt window");
		return false;
	}
	activate_window(L_LINK_PREV(curwp));
	return true;
}

/*
 *	split window in vertical direction 
 *	(it will make current buffer into two pieces)
 */
int window_vertinal_split(int f,int n)
{
	//if (curbp->nrow < 5) {
	//	showmsg(true,"Cant split under 5 line buffer");
	//	return false;
	//}
	//// init new buffer
	//buffer *bf = init_buffer(NULL,0,FREDRW);
	//curbp->nrow = curbp->nrow / 2;
	//bf->mtop = curbp->mtop + curbp->nrow - 1;
	//bf->nrow = curbp->nrow + 1;
	//curbp->flags |= FREDRW;
	//curbp->nrow -= 1;
	//append_buffer(curwp,bf);
	return true;
}
