#ifndef _BUFFER_H
# define _BUFFER_H

/*
 *	Manager buffers
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

#define ALONEBUFFER 0x002

#define NFILEN  256			/* n of bytes, file name 	*/
#define NBUFN   32			/* n of bytes, buffer name 	*/

struct buffer_t {
	L_LINK(buffer) link;	/* buffers doubly-link list */
	line *fline;			/* first line of buffer , to get doubly-linked list of lines */
	line *lline;			/* last line of buffer 										 */
	line *hline;			/* header line in this view (or page) 						 */
	line *cline;			/* current line in buffer (where the cursor is) 			 */
	int clindex;			/* current line index 										 */
	int lcount;				/* total count of buffer lines 								 */
	int loffset;			/* passed lines (how much we scrolled down)					 */
	int nrow;				/* n number of row avaiable in this buffer					 */
	char fname[NFILEN];		/* file name (full path of name)							 */
	char bname[NBUFN];		/* buffer name (just for show , contains just file name)	 */
	short flags;			/* flags for buffer like size flags and ...					 */
	ushort modes;			/* modes of this buffer 									 */
	int coffset;			/* char offset in line ( how many char passed )				 */
	int mtop;				/* buffer margin top from window							 */
	int mleft;				/* margin left from window									 */
	int dirty;				/* how much this buffer become dirty						 */
	// there some options for tab
	bool linenm;			/* activate line number for this buffer or no				 */
	bool highlight;			/* have syntax highlight or no :)							 */
	char *filetype;
};

#define sbprev(b,p) (b->link.prev = p)	/* set prev buffer for given buffer */
#define bmtest(b,m)	(b->modes & m)		/* test if buffer is in given mode 	*/
#define usmode(b,m)	(b->modes &= ~m)	/* unset a mode from buffer modes	*/
#define stmode(b,m)	(b->modes |= m)		/* set mode for buffer 				*/

/* buffer flags */
#define FFULLS	0x0001 	/* full size buffer  */
#define FVTBUF	0x0002	/* vertical buffer	 */
#define FHRBUF	0x0004	/* horizontal buffer */
#define FREDRW	0x0008	/* buffer need redraw*/

/* mode for buffers */
#define	MDLOCK	0x0001	/* lock mode                     */
#define MDINST  0x0002	/* insert mode					 */
#define MDVISL 	0x0004	/* visual mode					 */
#define	MDVIEW	0x0008	/* read-only buffer              */
#define	MDASAVE	0x0010	/* auto-save mode                */
#define MDCMMD 	0x0020	/* typing command mode			 */

/* this is usefull for macros , when they are avaiable in all modes */
#define ALLMODES (MDLOCK | MDINST | MDVISL | MDVIEW)

//#define	MDCMOD	0x0002		/* c indentation and fence match */
//#define	MDSPELL	0x0004		/* spell error parsing           */
//#define	MDEXACT	0x0008		/* exact matching for searches   */
//#define MDOVER	0x0020		/* overwrite mode                */
//#define MDMAGIC	0x0040		/* regular expressions in search */

/*
 *	alloc and create new buffer struct and return pointer
 */
buffer *init_buffer(char *, short, short);

/*
 *	set bname and fname for buffer by given path
 */
void set_buffer_name(char *);

/*
 *	activate buffer by given name , return false if no any buffer founded
 */
int set_buffer_by_name(char *);

/*
 *	works that we need to do when buffer changed
 */
void buffer_changed();

/*
 *	show a buffer and set it options and ..
 */
void change_current_buffer(buffer *);

/*
 *	append buffer to next of current buffer
 */
void append_buffer(window*,buffer *);

/*
 *	return last line of buffer
 */
line *get_last_line(buffer *);

/*
 *	load syntax file for buffer
 */
void load_syntax_for_buffer();

/*
 *	toggle line number option (it's global)
 */
int toggle_linenumber();

void toggle_highligth();

void set_mode_for_buffer(int);
int set_lock_mode(int, int);
int set_visual_mode(int, int);
int set_insert_mode(int, int);
int set_command_mode(int, int);

int next_buffer_in_window(int,int);
int prev_buffer_in_window(int,int);

/*
 *	this ones used to remove current buffer and active next buffer
 */
int remove_buffer();

/*
 *	make buffers lines free
 */
void free_buffer();

buffer *get_buffer_by_index(int);

#endif
