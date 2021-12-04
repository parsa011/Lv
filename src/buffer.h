#ifndef _BUFFER_H
# define _BUFFER_H

/*
 *	Manager buffers 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define NFILEN  256		/* n of bytes, file name 	*/
#define NBUFN   16		/* n of bytes, buffer name 	*/

typedef struct buffer_t {
	line *fline;			/* first line of buffer , to get doubly-linked list of lines */
	line *hline;			/* header line in this view (or page) 						 */
	line *cline;			/* current line in buffer (where the cursor is) 			 */
	char fname[NFILEN];		/* file name 												 */
	char bname[NBUFN];		/* buffer name 												 */
	char flag;				/* flags 													 */
	char mode;				/* modes of this buffer 									 */
	int loffset;			/* paased lines 											 */
};

#define	MDLOCK	0x0001		/* lock mode                     */
#define	MDCMOD	0x0002		/* c indentation and fence match */
#define	MDSPELL	0x0004		/* spell error parcing           */
#define	MDEXACT	0x0008		/* exact matching for searches   */
#define	MDVIEW	0x0010		/* read-only buffer              */
#define MDOVER	0x0020		/* overwrite mode                */
#define MDMAGIC	0x0040		/* regular expresions in search  */
#define	MDCRYPT	0x0080		/* encrytion mode active         */
#define	MDASAVE	0x0100		/* auto-save mode                */

#endif
