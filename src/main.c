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
	global_editor.tty_in = STDIN_FILENO;
	atexit(at_exit);
}

int main(int argc, char *argv[])
{
	init_editor();
	terminal_raw_mode();
	int c;
	do {
		c = read_char_from_terminal();
		if (IS_CTRL_KEY(c))
			printf("constrol key\n");
		printf("%c\n", c);
	} while (c != 'q');
	exit(0);
	return 0;
}
