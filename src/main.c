#include "lv.h"

void usage(char *program_name)
{
	printf("%s : %s [file_name]\n", program_name, program_name);
}

void at_exit()
{
	terminal_cooked_mode();
	clear_screen();
}

void init_editor()
{
	clear_screen();
	terminal_raw_mode();
	global_editor.tty_in = STDIN_FILENO;
	update_screen_size();
	atexit(at_exit);
}

int main(int argc, char *argv[])
{
	init_editor();
	int c;
	do {
		c = read_char_from_terminal();
		if (c == 'h')
			hide_cursor();
		else if (c == 's')
			show_cursor();
		else if (c == 'w') {
			cursor_position pos;
			get_cursor_pos(&pos);
			print_pos(pos);
		} else
			printf("%c", c);
		putchar('\n');
	} while (c != 'q');
	exit(0);
	return 0;
}
