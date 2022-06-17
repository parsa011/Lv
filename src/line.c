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
