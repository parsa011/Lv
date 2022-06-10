#ifndef _LINE_H
# define _LINE_H

/*
 * line struct is our text structure, that means we hold each line in a double linked list
 */
struct line_t {
	L_LINK(buffer) link;
	char *chars;
	int len;
};

#endif
