#include "lv.h"

void usage(char *program_name)
{
	printf("%s : %s [file_name]\n", program_name, program_name);
}

void at_exit()
{
	terminal_cooked_mode();
	tty_clear_screen();
}

void init_editor()
{
	tty_clear_screen();
	terminal_raw_mode();
	global_editor.tty_in = STDIN_FILENO;

	reset_pos(current_window.cursor_pos);
	tty_move_cursor(current_window.cursor_pos);

	setbuf(stdout, NULL);
	update_screen_size();
	current_buffer = &current_window.first_buffer;
	atexit(at_exit);
}

void lv_loop()
{
	int c;
	do {
		update_screen();
		c = get_key();
		if (c == CTRL_KEY('n')) {
			next_line();
		} else if (c == CTRL_KEY('l')) {
			go_line_end();
		} else if (c == CTRL_KEY('v')) {
			page_down();
		} else if (c == META_KEY('v')) {
			page_up();
		} else if (c == CTRL_KEY('h')) {
			go_line_beginning();
		} else if (c == CTRL_KEY('p')) {
			prev_line();
		} else if (c == CTRL_KEY('f')) {
			next_char();
		} else if (c == CTRL_KEY('b')) {
			prev_char();
		} else if (c == META_KEY('f')) {
			next_word();
		} else if (c == META_KEY('b')) {
			prev_word();
		} else if (!IS_CTRL_KEY(c)) {
			line_insert_char(c, current_buffer->char_offset);
		}
	} while (c != 'q');
}

int main(int argc, char *argv[])
{
	/* handy configs for now */
	global_editor.show_tabs = true;
	global_editor.tab_size = 8;

	init_editor();
	buffer_open_file(current_buffer, argv[1]);
	lv_loop();
	exit(0);
	return 0;
}
