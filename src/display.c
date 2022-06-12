#include "types.h"

#include <sys/ioctl.h>

public void update_screen()
{
	write_tildes();
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
