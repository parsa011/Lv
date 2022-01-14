#ifndef _LINE_H
# define _LINE_H

/*
 *	Line and rows declerations
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	all lines of buffer and ... , will stored in this struct , this is doubly-linked list 
 *	that means each line contains its next and prev line (they will stored in link prop) 
 *	TODO : in future we need add more property like syntax highlight flags
 */
struct line_t {
	LINK(line) link;	/* Doubly-linked list of lines for a particular buffer */
	char *chars;		/* content of line , actually a bunch of chars :) */
	int len;			/* length of current line */
};

#define lnext(lp)       ((lp)->link.next)
#define lprev(lp)       ((lp)->link.prev)
#define lgetc(lp, n)    ((lp)->chars[(n)] & 0xFF)
#define lputc(lp, n, c) ((lp)->chars[(n)] = (c))
#define llength(lp)     ((lp)->len)
#define slnext(lp,n) 	((lp)->link.next = n)	/* set next line for given line */ 
#define slprev(lp,p) 	((lp)->link.prev = p)	/* set prev line for given line */

/*
 * 	This routine allocates a block of memory large enough to hold a struct line
 * 	containing "used" characters. The block is always rounded up a bit. Return
 * 	a pointer to the new block, or NULL if there isn't any memory left. Print a
 * 	message in the message line if no space.
 */
line *line_alloc(char *, int);

/*
 *	appden line to buffer and change last line and ...
 *	if given buffer is NULL , we will count buffer as curbp
 */
int append_line(buffer *,line *);

/*
 * calculate line length with tabs (convert a tab char to its specified size in tab_size)
 */
int line_length(line *);

/*
 *	delete char in current position of cursor
 */
void line_del_char();

/*
 *	insert given char into current position of cursor
 */
void line_ins_char(char);
void line_del_next();
int delete_current_char(int,int);

/*
 *	append given string in current position of cursor
 */
void line_append(line *,char *,int);

/*
 *	delete a line , by the line number
 *	we should connect next line prev (lnext(line)->link-prev) to given line next
 */
void line_delete(int);
int delete_current_line(int,int);

/*
 *	alloc new , and insert it in current position of cursor
 */ 
int line_new(int);

int line_new_down(int,int);
int line_new_up(int,int);

/*
 *	replace current char under the cursor with given char
 */
void line_rep_char(char);

line *get_line_by_index(int);
#endif
