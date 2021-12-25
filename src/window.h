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
	LINK(window) link;		/* Doubly-linked list of windows 			*/
	buffer *fbuffer;		/* buffers in this window 					*/
	int bcount;				/* total buffers count in this window 		*/
	int cbindex;			/* index of current buffer (active buffer)  */	
	char flags;				/* flags that holds state of window 		*/
	int crow;				/* cursor row in this window				*/
	int ccol;				/* cursor col								*/
};

#define wnext(w) 	 (w->link.next)		/* next window of given window 		*/
#define wprev(w) 	 (w->link.prev)		/* prev window of given window 		*/
#define swnext(wp,n) ((wp)->link.next = n)	/* set next window for given window */ 
#define swprev(wp,p) ((wp)->link.prev = p)	/* set prev window for given window */
#define wusmode(b,m) (b->flags &= ~m)	/* unset a flag for window			*/
#define wstmode(b,m) (b->flags |= m)		/* set flag for window				*/

/* window modes */
#define WFFORCE 0x01	/* window needs to be forced reframe  */
#define WFMOVE  0x02	/* movement from line to line   */
#define WFEDIT  0x04	/* editing within a line        */
#define WFHARD  0x08	/* better to a full display     */

#define ALONEWINDOW 0x02 /* needed in remove window and .. */

/*
 *	create ana alloc new window and return that pointer
 */
window *init_window();

/*
 *	delete a window and remove its buffers and lines and ...
 */
int remove_window(window *);

/*
 *	return last buffer of given window
 */
buffer *get_last_buffer(window *);

void activate_window(window *);

int append_window(window *);

/* go to next window */
int next_window(int,int);
/* go to prev window */
int prev_window(int,int);



#endif
