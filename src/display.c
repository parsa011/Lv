#include "types.h"

#include <sys/ioctl.h>

public void update_screen()
{
	cursor_position cur_pos;
	pos_copy(current_window.cursor_pos, cur_pos);
	update_tabbar();
	update_text();
	update_command_bar();
	tty_move_cursor(cur_pos);
}


public void update_text()
{
	tty_hide_cursor();
	/* calculate that how much row we have for text lines */
	int text_region_size = global_editor.term_row - global_editor.show_tabs;
	/*
	 *	place cursor to the actuall place for texts section
	 */
	tty_move_cursor(CURSOR_POS(1 + global_editor.show_tabs, 1));
	line *ln = buffer_get_line_by_index(current_buffer, current_buffer->line_offset);
	for (int i = 0; i < text_region_size - 1; i++) {
		tty_erase_end_of_line();
		if (ln) {
			printf("%s\n", ln->chars);
			ln = L_LINK_NEXT(ln);
		} else {
			printf("~ \n\r");
		}
	}
	tty_show_cursor();
}

public void update_tabbar()
{
	if (!global_editor.show_tabs)
		return;
	tty_hide_cursor();
	
	tty_show_cursor();
	reset_color();
}

public void update_command_bar()
{
	tty_hide_cursor();
	tty_move_cursor(CURSOR_POS(global_editor.term_row, 1));
	tty_erase_end_of_line();

	printf("Line Count : %ld ------- Line Offset : %ld ------ Current Line Index : %ld ----- Cursor Pos : ", current_buffer->line_count,
		   current_buffer->line_offset, current_line_index());
	print_pos(current_window.cursor_pos);
	putchar('\r');
	tty_show_cursor();
}

public void paint_line(char *color)
{
	change_color(color);

}

void update_screen_size()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	global_editor.term_col = w.ws_col;
	global_editor.term_row = w.ws_row;
}
