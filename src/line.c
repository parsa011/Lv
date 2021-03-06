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
		len--;
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
	ln->chars = realloc(ln->chars, ln->len + 1);
	shift_right(ln->chars, ln->len, offset);
	line_put_char(ln, c, offset);
	line_put_char(ln, 0, ln->len);
	next_char();
	buffer_modified();
}

public void line_insert_string(line *ln, char *string, int len)
{
	ln->chars = realloc(ln->chars, ln->len + len + 1);
	strcat(ln->chars, string);

	ln->len += len;
	line_put_char(ln, 0, ln->len);
}

public void line_insert_new()
{
	line *current = buffer_current_line();
	if (!current) {
		buffer_line_append(current_buffer, line_init("", 0));
	} else {
		int offset = current_buffer->char_offset;
		line *new = line_init(current->chars + offset, current->len - offset);
		line_put_char(current, 0, offset);
		current->len = offset;
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
		int prev_len = current->link.prev->len;
		line_insert_string(current->link.prev, current->chars, current->len);
		line_remove(current);
		go_to_col(prev_len);
		goto ret;
	}
	/*	This should be here, because if last char was \t this should control our offset
	 */
	prev_char();
	shift_left(current->chars, current->len, offset - 1);
	current->len--;
	line_put_char(current, 0, current->len);
ret :
	buffer_modified();
}

public void line_delete_word()
{
	/* to determine if current char is alpha or no */
	bool is_alpha;
#define UPDATE_CH() {													\
		is_alpha = isalpha(*(buffer_current_line()->chars + current_buffer->char_offset)); \
	}
	UPDATE_CH();
	while (!is_alpha) {
		next_char();
		line_delete_char();
		UPDATE_CH();
	}
	while (is_alpha) {
		next_char();
		line_delete_char();
		UPDATE_CH();
	}
#undef UPDATE_CH
}

public void line_delete_after(int offset)
{
	line *ln = buffer_current_line();
	if (offset == 0 && ln->len == 0)
		line_remove(ln);
	else {
		ln->chars = realloc(ln->chars, offset + 1);
		ln->len = offset;
		line_put_char(ln, 0, ln->len);
	}
	buffer_modified();
}

public bool line_in_word()
{
	if (buffer_current_line() == NULL)
		return false;
	int c;
	if (current_buffer->char_offset > buffer_current_line()->len)
		return false;
	c = *(buffer_current_line()->chars + current_buffer->char_offset);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return true;
	return false;
}

public void line_remove(line *ln)
{
	if (ln == current_buffer->first_line) {
		next_line();
		go_line_beginning();
		line_delete_char();
		return;
	} else
		prev_line();
	L_LINK_REMOVE(ln);
	line_free(ln);
	current_buffer->line_count--;
}

public void line_free(line *ln)
{
	free(ln->chars);
	free(ln);
}
