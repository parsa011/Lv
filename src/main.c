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
	current_buffer = &current_window.first_buffer;
	atexit(at_exit);
}

int main(int argc, char *argv[])
{
	init_editor();
	int c;
	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}
	buffer_open_file(current_buffer, argv[1]);
	line *ln = current_buffer->first_line;
	while (ln) {
		printf("%s", ln->chars);
		ln = L_LINK_NEXT(ln);
	}
	do {
		c = get_key();
	} while (c != 'q');
	exit(0);
	return 0;
}
