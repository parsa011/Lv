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
		if (c == 'j') {
			next_line();			
		} else if (c == 'k') {
			prev_line();
		} else if (c == 'l') {
			next_char();
		} else if (c == 'h') {
			prev_char();
		}
	} while (c != 'q');
}

int main(int argc, char *argv[])
{
	/* handy configs for now */
	global_editor.show_tabs = true;

	init_editor();
	buffer_open_file(current_buffer, argv[1]);
	lv_loop();
	exit(0);
	return 0;
}
