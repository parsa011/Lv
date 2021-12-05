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
	buffer *buffers;		/* buffers in this window 					*/
	int bcount;				/* total buffers count in this window 		*/
	//buffer *cb;			/* current buffer (active) 					*/
	int cbindex;			/* index of current buffer (active buffer)  */	
	char flags;				/* flags that holds state of window 		*/
};

#define wnext(w) (w.link->next)				/* next window of given window 		*/
#define wprev(w) (w.link->prev)				/* prev window of given window 		*/
#define wcbuffer(w) (w.buffers[w.cb_index]) /* return current buffer in window 	*/

/* window modes */
#define WFFORCE 0x01		/* window needs forced reframe  */
#define WFMOVE  0x02		/* movement from line to line   */
#define WFEDIT  0x04		/* editing within a line        */
#define WFHARD  0x08		/* better to a full display     */
#define WFMODE  0x10		/* update mode line.            */
#define	WFCOLR	0x20		/* needs a color change         */

#endif