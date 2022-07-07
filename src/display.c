#include "types.h"
#include <time.h>

#include <sys/ioctl.h>

public void update_screen()
{
	cursor_position cur_pos;
	pos_copy(current_window->cursor_pos, cur_pos);
	update_tabbar();
	update_text();
	update_status_bar();
	update_command_bar();
	tty_move_cursor(cur_pos);
}


public void update_text()
{
	if (!current_buffer->need_text_update)
		return;
	tty_hide_cursor();
	/* calculate that how much row we have for text lines */
	int text_region_size = TEXT_ROW_SIZE;
	/*
	 *	place cursor to the actuall place for texts section
	 */
	tty_move_cursor(CURSOR_POS(1 + global_editor.show_tabs, 1));
	line *ln = buffer_get_line_by_index(current_buffer, current_buffer->line_offset);
	for (int i = 0; i < text_region_size; i++) {
		tty_erase_end_of_line();
		write_line(ln);
		if (ln)
			ln = L_LINK_NEXT(ln);
	}
	current_buffer->need_text_update = false;
	ttyflush();
	tty_show_cursor();
}

public void write_line(line *ln)
{
	if (ln) {
		char *ptr = ln->chars;
		while (*ptr) {
			if (*ptr == '\t') {
				for (int i = 0; i < global_editor.tab_size; i++) {
					ttyputc(' ');
				}
			} else {
				ttyputc(*ptr);
			}
			ptr++;
		}
		ttyputs("\n", true);
	} else {
		ttyputs("~ \n\r", false);
	}
}

public void update_tabbar()
{
	if (!global_editor.show_tabs)
		return;
	tty_hide_cursor();
	tty_move_cursor(CURSOR_POS(1, 1));
	tty_erase_end_of_line();
	change_color(create_rgb_color(TABBAR_FG, TABBAR_BG));
#define ADD_TEXT(s) {			\
	bufp += sprintf(bufp, s);	\
}
#define ADD_TEXTF(s, ...) {		\
	bufp += sprintf(bufp, s, __VA_ARGS__);	\
}
	char buf[global_editor.term_col];
	char *bufp = buf;
	for (window *win = window_first(); win != NULL; win = L_LINK_NEXT(win)) {
		ADD_TEXTF("%s", win->first_buffer->file_name);
		if (L_LINK_NEXT(win))
			ADD_TEXT(" | ");
	}
	int space = global_editor.term_col - (bufp - buf);
	ttyputs(buf, true);
	for (int i = 0; i < space; i++) {
		ttyputc(' ');
	}
	ttyflush();
	tty_show_cursor();
	reset_color();
#undef ADD_TEXT
#undef ADD_TEXTF
}

public void update_status_bar()
{
	tty_hide_cursor();
	tty_move_cursor(CURSOR_POS(global_editor.term_row - 1, 1));
	tty_erase_end_of_line();
	change_color(create_rgb_color(TABBAR_FG, TABBAR_BG));
	char buf[global_editor.term_col];
	char *bufp = buf;
#define ADD_TEXT(s) {									\
	bufp += sprintf(bufp, s);							\
}
#define ADD_TEXTF(s, ...) {							\
	bufp += sprintf(bufp, s, __VA_ARGS__);			\
}
#if DEBUG == false
	ADD_TEXT("LV-Editor ");
	if (current_buffer->is_modified) {
		ADD_TEXT("*");
	} else
	ADD_TEXT("-");
	ADD_TEXTF(" %s", current_buffer->file_name);
	ADD_TEXTF(" ----- %ld Line ", current_buffer->line_count);
#else
	ADD_TEXTF("Line Count : %ld -- Line Offset : %ld -- "
		   "Current Line Index : %ld -- Char Offset : %d -- Cursor Pos : ",
		   current_buffer->line_count,
		   current_buffer->line_offset, buffer_line_index(), current_buffer->char_offset);
	print_pos(current_window->cursor_pos);
	if (buffer_current_line()) {
		char *c = buffer_current_line()->chars + current_buffer->char_offset;
		ADD_TEXTF(" --- Current char : %c", *c == '\t' ? 'T' : *c);
		ADD_TEXTF(" --- Line Length : %d", buffer_current_line()->len );
	}
#endif
	int space = global_editor.term_col - strlen(buf);
	ttyputs(buf, true);
	for (int i = 0; i < space; i++) {
		ttyputc('-');
	}
	ttyflush();
	reset_color();
	tty_show_cursor();
#undef ADD_TEXT
#undef ADD_TEXTF
}

public void update_command_bar()
{
	if ((time(NULL) - user_message_time > USER_MESSAGE_TIME)) {
		tty_move_cursor(CURSOR_POS(global_editor.term_row, 1));
		tty_erase_end_of_line();
	}
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
