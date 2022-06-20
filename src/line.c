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
	if (!ln) {
		line_insert_new();
		ln = buffer_current_line();
	}
	ln->len++;

	ln->chars = realloc(ln->chars, ln->len);
	memmove(ln->chars + offset + 1, ln->chars + offset, ln->len - offset - 1);
	line_put_char(ln, c, offset);
	line_put_char(ln, 0, ln->len - 1);
	
	next_char();
	buffer_modified();
}

public void line_insert_string(line *ln, char *string, int len)
{
	ln->chars = realloc(ln->chars, ln->len + len - 1);
	memcpy(ln->chars + ln->len - 1, string, len);
	ln->len += len - 1;
	line_put_char(ln, '\0', ln->len - 1);
}

public void line_insert_new()
{
	line *current = buffer_current_line();
	if (!current) {
		buffer_line_append(current_buffer, line_init("", 0));
	} else {
		int offset = current_buffer->char_offset;
		line *new = line_init(current->chars + offset, current->len - offset);
		line_put_char(current, '\0', offset);
		current->len = offset + 1;
		buffer_line_append_after(current_buffer, current, new);
		next_line();
		go_line_beginning();
	}
	buffer_modified();
}

public void line_delete_char()
{
	int offset = current_buffer->char_offset;
	line *current = buffer_current_line();
	if (offset == 0 ) {
		if (buffer_line_index() == 0)
			return;
		line_insert_string(current->link.prev, current->chars, current->len);
		line_remove(current);
		goto ret;
	}
	prev_char();
	memcpy(current->chars + offset - 1, current->chars + offset, current->len - offset);
	current->len--;
	line_put_char(current, '\0', current->len);
ret :
	buffer_modified();
}

public void line_remove(line *ln)
{
	prev_line();
	go_line_end();
	L_LINK_REMOVE(ln);
	line_free(ln);
	current_buffer->line_count--;
}

public void line_free(line *ln)
{
	free(ln->chars);
	free(ln);
}
