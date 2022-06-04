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
	L_LINK(line) link;	/* Doubly-linked list of lines for a particular buffer */
	char *chars;		/* content of line , actually a bunch of chars :) */
	int len;			/* length of current line */
};

#define lgetc(lp, n)    ((lp)->chars[(n)] & 0xFF)
#define lputc(lp, n, c) ((lp)->chars[(n)] = (c))
#define llength(lp)     ((lp)->len)

/* global line indent value , each time that we want to insesrt new line , we will add space 
 * to this amount */
extern int line_indent;

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
 *	Remove Previous chars
 */
void delete_prev_char();

/*
 *	delete char in current position of cursor
 */
void delete_next_char();

/*
 *	insert given char into current position of cursor
 */
void line_ins_char(char);

/*
 *	delete next char of current cursor position
 */
void line_del_next();

/*
 *	remove currnet character under cursor
 */
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

/*
 *	remove line from buffer
 * 	TODO : add undo
 */
int delete_current_line(int,int);

/*
 *	alloc new , and insert it in current position of cursor
 */ 
int line_new(int);

/*
 * these two function are used for macros o and O to open new line in up and down
 */
int line_new_down(int,int);
int line_new_up(int,int);

/*
 *	replace current char under the cursor with given char
 */
void line_rep_char(char);

/*
 *	return line by its index in current buffer
 */
line *get_line_by_index(int);

/*
 *	update line indent value , in cant we will count prev lines space at beginning of line
 */ 
void update_indent();

/*
 *	update and insert indent to line
 */
void insert_indent();

#endif
