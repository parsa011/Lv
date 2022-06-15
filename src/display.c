#include "types.h"

#include <sys/ioctl.h>

public void update_screen()
{
	if (global_editor.show_tabs)
		update_tabbar();
	update_text();
}

public void update_text()
{
	int text_region_size = global_editor.term_row - global_editor.show_tabs;
	line *ln = current_buffer->first_line;
	for (int i = 0; i < text_region_size - 1; i++) {
		if (ln) {
			printf("%s\n", ln->chars);
			ln = L_LINK_NEXT(ln);
		} else {
			printf("~ \n\r");
		}
	}
}

public void update_tabbar()
{
	tty_hide_cursor();
	tty_move_cursor(CURSOR_POS(1, 1));
	paint_line(TABBAR_BG);
	change_color(TABBAR_BG);
	change_color(TABBAR_FG);
	printf("LV ");
	for (window *fw = &current_window; fw; fw = L_LINK_NEXT(fw)) {
		for (buffer *buf = &fw->first_buffer; buf; buf = L_LINK_NEXT(buf)) {
			if (buf == current_buffer)
				set_bold();
			printf("[ %s ] ", buf->file_name);
			if (buf == current_buffer)
				unset_bold();
		}
	}
	printf("\n\r");
	tty_show_cursor();
	reset_color();
}

public void paint_line(char *color)
{
	change_color(color);
	cursor_position current_pos;
	pos_copy(current_window.cursor_pos, current_pos);
	for (int i = 0; i < global_editor.term_col; i++) {
		putchar(' ');
	}
	putchar('\r');
	tty_move_cursor(current_pos);
	reset_color();
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
