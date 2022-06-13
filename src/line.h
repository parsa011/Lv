#ifndef _LINE_H
# define _LINE_H

/*
 * line struct is our text structure, that means we hold each line in a double linked list
 */
struct line_t {
	L_LINK(line) link;
	char *chars;
	int len;
};

/*
 *	set chars and len for give line
 *	NOTE : Do note take duplicate of line chars before using this function
 *	We will do it 
 */
public line *line_init(char *chars, int len);

#endif
