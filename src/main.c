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
		c = get_key();
		//print_key(c);
	} while (c != 'q');
	exit(0);
	return 0;
}
