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
	/* but it should't be NULL */
	if (!current_line)
		return;
	if (current_line->len < current_buffer->char_offset)
		current_buffer->char_offset = current_line->len > 0 ? current_line->len - 1 : 0;
	cursor_col = offset_to_col(buffer_current_line()->chars, current_buffer->char_offset);
}

public bool next_line()
{
	/*
	 *	prevent of extra scroll down or line move, if current line index	
	 *  is more than line_count that means we dont have any other line , so dont
	 *	scroll or move cursor buddy :)
	 */
	if (buffer_line_index() >= current_buffer->line_count - 1)
		return false;
	if (cursor_row == global_editor.term_row - 2) {
		current_buffer->line_offset++;
		buffer_text_update();
		goto ret;
	}
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
			buffer_text_update();
			current_buffer->line_offset--;
			buffer_text_update();
			goto ret;
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
	if (current_buffer->char_offset <= ln->len - 1) {
		char current_char = *(ln->chars + current_buffer->char_offset);
		if (current_char == '\t')
			cursor_col += global_editor.tab_size;
		else
			cursor_col++;
		current_buffer->char_offset++;
		return true;
	} else {
		if (next_line()) {
			go_line_beginning();
			return true;
		}
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
		if (prev_line()) {
			go_line_end();
			return true;
		}
	}
	return false;
}

public bool go_line_end()
{
	current_buffer->char_offset = buffer_current_line()->len;
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

public bool next_word()
{
	while (line_in_word() == true) {
		if (next_char() != true)
			return false;
	}
	// escape of spaces , to go to the start of next word
	while (line_in_word() == false) {
		if (next_char() != true)
			return false;
	}
	return true;
}

public bool prev_word()
{
	// escape of chars
	while (line_in_word() == true) {
		if (prev_char() != true)
			return false;
	}
	// escape of spaces , to go to the start of next word
	while (line_in_word() == false) {
		if (prev_char() != true)
			return false;
	}
	return true;
}

public void go_to_col(int col)
{
	current_buffer->char_offset = col;
	control_offset();
}

public bool go_to_line(uint64_t line_nu)
{
	if (line_nu > current_buffer->line_count - 1)
		return false;
	line *ln = buffer_get_line_by_index(current_buffer, line_nu);
	reset_pos(current_window->cursor_pos);
	current_buffer->current_line = ln;
	current_buffer->line_offset = line_nu;
	buffer_text_update();
	return true;
}
