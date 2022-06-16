#include "types.h"

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
			return true;
		}
		return false;
	}
	/* no any line, dont move cursorr */
	if (current_buffer->line_count <= current_buffer->line_offset)
		return false;
	tty_cursor_next_line();
	cursor_row++;
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
			return true;
		}
		return false;
	}
	tty_cursor_prev_line();
	cursor_row--;
	return true;
}

public bool next_char()
{
	line *ln = buffer_get_line_by_index(current_buffer, buffer_line_index());
	if (current_buffer->char_offset + 1 < ln->len) {
		current_buffer->char_offset++;
		cursor_col++;
		return true;
	}
	return false;
}

public bool prev_char()
{
	if (current_buffer->char_offset > 0) {
		current_buffer->char_offset--;
		cursor_col--;
		return true;
	}
	return false;
}
