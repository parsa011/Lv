#ifndef _TTY_H
# define _TTY_H

#include <termios.h>

private struct termios old;

public void terminal_raw_mode();

public void terminal_cooked_mode();

public void mouse_enable();

public void mouse_disable();

public void move_cursor(cursor_position cursor_pos);

public int get_cursor_pos(cursor_position *pos);

public void clear_screen();

public void hide_cursor();

public void show_cursor();

public void store_cursor();

public void restore_cursor();

#endif
