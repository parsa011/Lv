#include "types.h"

#include <sys/ioctl.h>

private struct termios old;


/* output buffer, index and size */

private unsigned char obuf[OBUFSIZE];
private int obufp = 0;

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

/* 
 * write string to output buffer
 */
void ttyputs(char *s, bool flush_now)
{
	while (*s) {
		ttyputc(*s++);
	}
	if (flush_now)
		ttyflush();
}

void ttyputc(char c)
{
	if (obufp == OBUFSIZE)
		ttyflush();
	obuf[obufp++] = c;
}

/* 
 * flush output and check for type ahead 
 */
int ttyflush()
{
	if (obufp) {
		// TODO : DIE HERE
		if (write(global_editor.tty_in, obuf, obufp) == 0)
			printf("SOMETHING WENT WRONG WHEN WRITING INTO TERMOUT\n");
		obufp = 0;
	}
	return true;
}

public int ttycheck()
{
	int x;
	return ((ioctl(0, FIONREAD, &x) == -1) ? 0 : x);
}

public void tty_mouse_enable()
{
	ttyputs("\033[?1006h\033[?1000h", true);
}

public void tty_mouse_disable()
{
	ttyputs("\033[?1006l\033[?1000l", true);
}

public void tty_move_cursor(cursor_position cursor_pos)
{
	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", cursor_pos.row, cursor_pos.col);
	ttyputs(buf, true);
}

/*
 * Use the ESC [6n escape sequence to query the horizontal cursor position
 * and return it. On error -1 is returned, on success the position of the
 * cursor is stored at *rows and *cols and 0 is returned. 
 */
public int tty_get_cursor_pos(cursor_position *cursor_pos)
{
    char buf[32];
    unsigned int i = 0;

    /* Report cursor location */
    if (write(global_editor.tty_in, "\x1b[6n", 4) != 4)
		return -1;

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
    if (sscanf(buf + 2, "%hhd;%hhd", &cursor_pos->row, &cursor_pos->col) != 2)
		return -1;
    return 0;
}

public void tty_clear_screen()
{
	ttyputs("\033[H\033[2J", true);
}

public void tty_hide_cursor()
{
	ttyputs("\033[?25l", true);
}

public void tty_show_cursor()
{
	ttyputs("\033[?25h", true);
}

public void tty_store_cursor()
{
	ttyputs("\0337", true);
}

public void tty_restore_cursor()
{
	ttyputs("\0338", true);
}

public void tty_erase_end_of_line()
{
	ttyputs("\x1b[2K", true);
}

public void tty_erase_of_age()
{
	ttyputs("\x1b[2J", true);
}

public void tty_cursor_next_line()
{
	ttyputs("\x1b[B", true);
}

public void tty_cursor_prev_line()
{
	ttyputs("\x1b[A", true);
}

public void tty_cursor_next_char()
{
	ttyputs("\x1b[C", true);
}

public void tty_cursor_prev_char()
{
	ttyputs("\x1b[D", true);
}
