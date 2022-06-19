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

public void line_insert_new()
{
	int offset = current_buffer->char_offset;
	line *current = buffer_current_line();
	line *new = line_init(current->chars + offset, current->len - offset);
	current->chars[offset] = 0;
	current->len = offset + 1;
	buffer_line_append_after(current_buffer, current, new);
	next_line();
	go_line_beginning();
	buffer_modified();
}
