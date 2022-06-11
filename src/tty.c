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


/*
 * Use the ESC [6n escape sequence to query the horizontal cursor position
 * and return it. On error -1 is returned, on success the position of the
 * cursor is stored at *rows and *cols and 0 is returned. 
 */
int get_cursor_pos(cursor_position *cursor_pos) {
    char buf[32];
    unsigned int i = 0;

    /* Report cursor location */
    if (write(global_editor.tty_in, "\x1b[6n", 4) != 4) return -1;

    /* Read the response: ESC [ rows ; cols R */
    while (i < sizeof(buf) -1) {
        if (read(global_editor.tty_in, buf + i, 1) != 1)
			break;
        if (buf[i] == 'R')
			break;
        i++;
    }
    buf[i] = '\0';

    /* Parse it. */
    if (buf[0] != ESC || buf[1] != '[')
		return -1;
    if (sscanf(buf + 2, "%d;%d", &cursor_pos->x, &cursor_pos->y) != 2)
		return -1;
    return 0;
}

public void clear_screen(void)
{
	write(global_editor.tty_in, "\033[H\033[2J", 8);
}

public void hide_cursor()
{
	write(global_editor.tty_in, "\033[?25l", 7);
}

public void show_cursor()
{
	write(global_editor.tty_in, "\033[?25h", 7);
}

public void store_cursor()
{
	write(global_editor.tty_in, "\0337", 3);
}

public void restore_cursor()
{
	write(global_editor.tty_in, "\0338", 3);
}
