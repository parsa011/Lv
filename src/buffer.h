#ifndef _BUFFER_H
# define _BUFFER_H

/*
 *	Manager buffers 
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

typedef struct buffer_t {
	line *first_line;		/* first line of buffer , to get doubly-linked list of lines */
} buffer;

#endif
