#include "types.h"

public line *line_init(char *chars, int len)
{
	line *ln = malloc(sizeof(line));
	assert(ln);
	ln->link.next = ln->link.prev = 0;
	ln->chars = strdup(chars);
	ln->len = len;
	return ln;
}
