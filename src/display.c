#include "types.h"

#include <sys/ioctl.h>

public void update_screen()
{
	update_tabbar();
}

public void update_tabbar()
{
	tty_hide_cursor();
	tty_move_cursor(CURSOR_POS(1, 1));
	paint_line(create_rgb_color(color_new(123, 123, 123), color_new(44, 44, 44)));
	tty_show_cursor();
}

public void paint_line(char *color)
{
	ttyputs(color, false);
	cursor_position current_pos;
	pos_copy(current_window.cursor_pos, current_pos);
	for (int i = 0; i < global_editor.term_col; i++) {
		ttyputc('*');
	}
	ttyputc('\r');
	tty_move_cursor(current_pos);
	ttyputs(COLORDEFAULT, true);
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
