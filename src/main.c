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
	update_screen_size();
	atexit(at_exit);
}

int main(int argc, char *argv[])
{
	init_editor();
	int c;
	write_tildes();
	tty_move_cursor(CURSOR_POS(1, 1));
	do {
		c = get_key();
		if (c == 'h')
			tty_cursor_prev_char();
		else if (c == 'j')
			tty_cursor_next_line();
		else if (c == 'k')
			tty_cursor_prev_line();
		else if (c == 'l')
			tty_cursor_next_char();
		else if (c == ':')
			tty_move_cursor(CURSOR_POS(global_editor.term_row, 1));
	} while (c != 'q');
	exit(0);
	return 0;
}
