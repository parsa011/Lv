#ifndef _LINE_H
# define _LINE_H

/*
 *	Line and rows declerations
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

struct line_t {
	LINK(line) link;	/* Doubly-linked list of lines for a particular buffer */
	char *chars;		/* content of line , actually a bunch of chars :) */
	int len;			/* length of current line */
	buffer *buf;		/* owner buffer of this line */
};

#define lnext(lp)       ((lp)->link->next)
#define lprev(lp)       ((lp)->link->prev)
#define lgetc(lp, n)    ((lp)->l_text[(n)] & 0xFF)
#define lputc(lp, n, c) ((lp)->l_text[(n)] = (c))
#define llength(lp)     ((lp)->l_len)

#endif
