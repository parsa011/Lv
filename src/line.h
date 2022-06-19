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
#define line_put_char(lp, c, n) ((lp)->chars[(n)] = (c))

/*
 *	set chars and len for give line
 *	NOTE : Do note take duplicate of line chars before using this function
 *	We will do it 
 */
public line *line_init(char *chars, int len);

/*
 *	append given char into given offset of current line
 */
public void line_insert_char(int c, int offset);

/*
 *	append string to end of given string  
 */
public void line_insert_string(line *ln, char *string, int len);

/*
 *	Insert new line after current line
 */
public void line_insert_new();

/*
 *	remove current char from current line (char under cursor)
 */
public void line_delete_char();

/*
 *	completely remove line from buffer and make it free 
 */
public void line_remove(line *ln);

/*
 *	make line free with its chars
 */
public void line_free();

#endif
