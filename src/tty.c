#include "types.h"

struct termios old;

public void terminal_raw_mode()
{
	tcgetattr(STDOUT_FILENO, &old);
	struct termios new = old;
	new.c_iflag &= (~ICRNL) & (~IXON);
	new.c_lflag &= (~ICANON) & (~ECHO) & (~ISIG);
#ifdef VLNEXT
	new.c_cc[VLNEXT] = 0;
#endif
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &new);
}

public void terminal_cooked_mode()
{
	tcsetattr(STDOUT_FILENO, TCSAFLUSH, &old);
}

public void mouse_enable()
{
	printf("\033[?1000h");
	printf("\033[?1006h");
}

public void mouse_disable()
{
	printf("\033[?1006l");
	printf("\033[?1000l");
}

public void move_cursor(cursor_position cursor_pos)
{
	printf("\033[%d;%dH", cursor_pos.y, cursor_pos.x);
}

public void clear_screen(void)
{
	printf("\033[H\033[2J");
}

public void hide_cursor()
{
	printf("\033[?25l");
}

public void show_cursor()
{
	printf("\033[?25h");
}

public void store_cursor()
{
	printf("\0337");
}

public void restore_cursor()
{
	printf("\0338");
}
