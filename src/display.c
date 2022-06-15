#include "types.h"

#include <sys/ioctl.h>

public void update_screen()
{
	cursor_position cur_pos;
	pos_copy(current_window.cursor_pos, cur_pos);
	update_tabbar();
	update_text();
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
	line *ln = current_buffer->first_line;
	for (int i = 0; i < text_region_size - 1; i++) {
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

public void paint_line(char *color)
{
	change_color(color);

}

public void write_tildes()
{
	tty_move_cursor(CURSOR_POS(1, 1));
	for (int i = 0; i < global_editor.term_row; i++) {
		printf("~");
		if (i != global_editor.term_row - 1)
			putchar('\n');
	}
}

void update_screen_size()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	global_editor.term_col = w.ws_col;
	global_editor.term_row = w.ws_row;
}
