#include "types.h"

public bool next_line()
{
	if (cursor_row >= global_editor.term_row - 1) {
		if (current_buffer->line_count - 1 > current_buffer->line_offset) {
			current_buffer->line_offset++;
			return true;
		}
		return false;
	}

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
}
