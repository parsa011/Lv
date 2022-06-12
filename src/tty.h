#ifndef _TTY_H
# define _TTY_H

#include <termios.h>

private struct termios old;

/* 
 * 	write a character to the output buffer.  If it becomes
 * 	full, call ttflush()
 */
#define OBUFSIZE 512
private int obufp; 			/* output buffer index */
private unsigned char obuf[OBUFSIZE]; /* output buffer */

/*
 *	Write given string into out put buffer using ttyputc()
 *	sometimes we need to write output immediatly , so we will pass
 *	'true' for flush_now to autocall for ttyflush()
 */
private void ttyputs(char *string, bool flush_now);

/*
 *	add given given into out put array
 *	call ttyflush() when we reached to OBUFSIZE size
 */
private void ttyputc(char ch);

/*
 *	Print output to terminal by calling write()
 */
private int ttyflush();

public void terminal_raw_mode();

public void terminal_cooked_mode();

/*
 *	checks if there is any other char to read from terminal or no
 *	(any pending char)
 */
public int ttycheck();
public void tty_mouse_enable();
public void tty_mouse_disable();
public void tty_move_cursor(cursor_position cursor_pos);
public int tty_get_cursor_pos(cursor_position *pos);
public void tty_clear_screen();
public void tty_hide_cursor();
public void tty_show_cursor();
public void tty_store_cursor();
public void tty_restore_cursor();
public void tty_erase_end_of_line();
public void tty_erase_of_age();
public void tty_cursor_next_line();
public void tty_cursor_prev_line();
public void tty_cursor_next_char();
public void tty_cursor_prev_char();

#endif
