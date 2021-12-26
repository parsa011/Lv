#ifndef _DISPLAY_H
# define _DISPLAY_H

/*
 *	these definitions will specify our grid offset from top of screen
 */
#if HAVE_WINDOWS_BAR == 1
#	define windowsbar_start_offset 1
#else
#	define windowsbar_start_offset 0
#endif
#define buffers_start_offset 	windowsbar_start_offset + 1
#define statusbar_start_offset 	term.t_mrow - 1
#define messagebar_start_offset term.t_mrow

/*
 *	Handle redisplay
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */

/*
 *	init terminal and trun on raw mode
 */
int init_term();

/*	
 *	get terminal size from system.
 */
void get_screen_size(int *,int *);

/*
 *	return current position of cursor 
 */
bool get_cursor_position(int *, int *);
void set_window_title(char *);

/*
 *	update screen , write buffers into screen and set cursor position
 */
void update();

/*
 *	write all exist windows into screen
 */
void write_windows();
#define WINDOWS_SEPARATOR " | "

/*
 * 	write current buffer into screen
 * 	TODO : write multiple buffers in screen
 */
void write_buffer();

void write_line(line *);
int write_linenumber(int,int);
int update_linenumber_padding();

/*
 *	write status bar into screen , the place that we will put information about 
 *	current state of editor and cursor and file
 */
void write_statusbar();

void write_messagebar();

/*
 *	show a message in messagebar
 */
void showmsg(bool,char *,...);

#endif
