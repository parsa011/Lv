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

void set_window_title(char *);

#endif
