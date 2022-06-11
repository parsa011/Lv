#include "types.h"

#include <sys/ioctl.h>

void update_screen_size()
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	global_editor.term_col = w.ws_col;
	global_editor.term_row = w.ws_row;
}
