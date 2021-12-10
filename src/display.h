#ifndef _DISPLAY_H
# define _DISPLAY_H

/*
 *	Handle redisplay
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*	
 *	get terminal size from system.
 */
void get_screen_size(int *,int *);

/*
 *	return current position of cursor 
 */
bool get_cursor_position(int *, int *);
bool move_cursor(int, int);
void set_window_title(char *);

/*
 *	update screen , write buffers into screen and set cursor position
 */
void update();

/*
 *	write all exist windows into screen
 */
void write_windows();

/*
 * 	write current buffer into screen
 * 	TODO : write multiple buffers in screen
 */
void write_buffer();

/*
 *	write status bar into screen , the place that we will put information about 
 *	current state of editor and cursor and file
 */
void write_statusbar();

void write_messagebar();

#endif
