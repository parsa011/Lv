#ifndef _BUFFER_H
# define _BUFFER_H

/*
 *	Manager buffers 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define NFILEN  256			/* n of bytes, file name 	*/
#define NBUFN   32			/* n of bytes, buffer name 	*/

struct buffer_t {
	LINK(buffer) link;		/* buffers doubly-link list */
	line *fline;			/* first line of buffer , to get doubly-linked list of lines */
	line *lline;			/* last line of buffer 										 */
	line *hline;			/* header line in this view (or page) 						 */
	line *cline;			/* current line in buffer (where the cursor is) 			 */
	int clindex;			/* current line index */
	int lcount;				/* total count of buffer lines 								 */
	int loffset;			/* paased lines 											 */
	char fname[NFILEN];		/* file name 												 */
	char bname[NBUFN];		/* buffer name 												 */
	//char flag;			/* flags 													 */
	char mode;				/* modes of this buffer 									 */
	int coffset;			/* char offset in line ( how many char passed )				 */
	int mtop;				/* buffer margin top from window							 */
	int mleft;				/* marging left frop window									 */
};

#define bnext(b) (b->link.next)			/* next buffer of given buffer 		*/
#define bprev(b) (b->link.prev)			/* prev buffer of given buffer 		*/
#define sbnext(b,n) (b->link.next = n)	/* set next buffer for given buffer */ 
#define sbprev(b,p) (b->link.prev = p)	/* set prev buffer for given buffer */

/* mode for buffers */
#define	MDLOCK	0x0001		/* lock mode                     */
#define	MDCMOD	0x0002		/* c indentation and fence match */
#define	MDSPELL	0x0004		/* spell error parcing           */
#define	MDEXACT	0x0008		/* exact matching for searches   */
#define	MDVIEW	0x0010		/* read-only buffer              */
#define MDOVER	0x0020		/* overwrite mode                */
#define MDMAGIC	0x0040		/* regular expresions in search  */
#define	MDCRYPT	0x0080		/* encrytion mode active         */
#define	MDASAVE	0x0100		/* auto-save mode                */

buffer *init_buffer(char *, char *, char);
void append_buffer(buffer *);
line *get_last_line(buffer *);

#endif
