#include "types.h"

public line *line_init(char *chars, int len)
{
	line *ln = malloc(sizeof(line));
	assert(ln);
	ln->link.next = ln->link.prev = 0;
	ln->chars = strdup(chars);
	/* remove new lines from end of string */
	char *ptr = ln->chars + len - 1;
    while (*ptr && *ptr == '\n') {
		*ptr-- = 0;
	}
	ln->len = len;
	return ln;
}

public void line_insert_char(int c, int offset)
{
	line *ln = buffer_current_line();
	ln->len++;

	ln->chars = realloc(ln->chars, ln->len);
	memmove(ln->chars + offset + 1, ln->chars + offset, ln->len - offset - 1);
	line_put_char(ln, c, offset);
	line_put_char(ln, 0, ln->len - 1);
	
	next_char();
	buffer_modified();
}
