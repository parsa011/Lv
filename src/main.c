#include "lv.h"

void usage(char *program_name)
{
	printf("%s : %s [file_name]\n", program_name, program_name);
}

int main(int argc, char *argv[])
{
	terminal_raw_mode();
	clear_screen();
	move_cursor(CURSOR_POS(12, 12));
	terminal_cooked_mode();
	return 0;
}
