#include "types.h"

public int offset_to_col(char *chars, int offset)
{
	int col = 1;
	for (int i = 0; i < offset; i++) {
		if (chars[i] == '\t')
			col += global_editor.tab_size;
		else
			col++;
	}
	return col;
}

public void control_offset()
{
	line *current_line = buffer_current_line();
	if (current_line->len <= current_buffer->char_offset)
		current_buffer->char_offset = current_line->len - 1;
	cursor_col = offset_to_col(buffer_current_line()->chars, current_buffer->char_offset);
}

public bool next_line()
{
	if (cursor_row >= global_editor.term_row - 1) {
		/*
		 *	prevent of extra scroll down, if term_row + line_offset and other stuff 
		 *	is more than line_count , that means we dont have any other line , so dont
		 *	scroll buddy :)
		 */
		if (current_buffer->line_offset + global_editor.term_row - 1 - global_editor.show_tabs
			 >
			current_buffer->line_count)
			return false;
		if (current_buffer->line_count - 1 > current_buffer->line_offset) {
			current_buffer->is_modified = true;
			current_buffer->line_offset++;
			goto ret;

			return true;
		}
		return false;
	}
	/* no any line, dont move cursorr */
	if (current_buffer->line_count <= current_buffer->line_offset)
		return false;
	tty_cursor_next_line();
	cursor_row++;
ret :
	current_buffer->current_line = L_LINK_NEXT(current_buffer->current_line);
	control_offset();
	return true;
}

public bool prev_line()
{
	if (cursor_row <= 1 + global_editor.show_tabs) {
		/* if cursor is in top of texts region , we check line_offset
		 * and if it is more than 0 , that means we passed some lines, 
		 * we will decrease it so we can get prev line to show
		 */
		if (current_buffer->line_offset > 0) {
			current_buffer->is_modified = true;
			current_buffer->line_offset--;
			goto ret;
			return true;
		}
		return false;
	}
	tty_cursor_prev_line();
	cursor_row--;
ret :
	current_buffer->current_line = L_LINK_PREV(current_buffer->current_line);
	control_offset();
	return true;
}

public bool next_char()
{
	line *ln = buffer_current_line();
	if (current_buffer->char_offset + 1 < ln->len) {
		char current_char = *(ln->chars + current_buffer->char_offset);
		if (current_char == '\t')
			cursor_col += global_editor.tab_size;
		else
			cursor_col++;
		current_buffer->char_offset++;
		return true;
	} else {
		return next_line();
	}
	return false;
}

public bool prev_char()
{
	line *ln = buffer_current_line();
	if (current_buffer->char_offset > 0) {
		current_buffer->char_offset--;
		char current_char = *(ln->chars + current_buffer->char_offset);
		if (current_char == '\t')
			cursor_col -= global_editor.tab_size;
		else
			cursor_col--;
		return true;
	} else {
		return prev_line();
	}		
	return false;
}

public bool go_line_end()
{
	line *ln = buffer_current_line();
	current_buffer->char_offset = ln->len - 1;
	control_offset();
	return true;
}

public bool go_line_beginning()
{
	line *ln = buffer_current_line();
	current_buffer->char_offset = 0;
	control_offset();
	return true;
}

public bool page_down()
{
	for (int i = 0; i < global_editor.term_row; i++) {
		next_line();
	}
	return true;
}

public bool page_up()
{
	for (int i = 0; i < global_editor.term_row; i++) {
		prev_line();
	}
	return true;
}

private int in_word()
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

bool next_word()
{
	while (in_word() == true) {
		if (next_char() != true)
			return false;
	}
	// escape of spaces , to go to the start of next word
	while (in_word() == false) {
		if (next_char() != true)
			return false;
	}
	return true;
}

bool prev_word()
{
	// escape of chars
	while (in_word() == true) {
		if (prev_char() != true)
			return false;
	}
	// escape of spaces , to go to the start of next word
	while (in_word() == false) {
		if (prev_char() != true)
			return false;
	}
	return true;
}
