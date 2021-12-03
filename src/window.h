#ifndef _WINDOW_H
# define _WINDOW_H

/*
 *	Manager windows	(and linux hahah)
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	there is a window structure allocated for every active display window
 *	the flag field contains some bits that are set by commands to guide
 * 	redisplay
 */
struct window_t {
	LINK(window) link;		/* Doubly-linked list of windows */
	buffer *buffers;		/* buffers in this window */
	int buffers_count;		/* total buffers count in this window */
	buffer *cb;				/* current buffer (active) */
	int cb_index;			/* index of cb */	
	char flags;				/* flags that holds state of window */
	char *wname;			/* window name for show in tabs section */
};

#define wnext(w) (w.link->next)
#define wprev(w) (w.link->prev)

#endif
