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

#define STATUSBAR_FILLER ' '
#define WINDOWS_SEPARATOR " | "

static struct rectangle *info_box;
static bool show_info_box;

/*
 *	init terminal and trun on raw mode
 */
int init_term();

/* set basic colors and ... */
int init_display();

/*	
 *	get terminal size from system.
 */
void get_screen_size(int *,int *);

/*
 *	return current position of cursor 
 */
bool get_cursor_position(int *, int *);

/*
 *	set title for terminal window
 */
void set_terminal_title(char *);

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
 * write given buffer lines to output, starts from header line , size of terminal row
 */
void write_buffer_lines(buffer *);

/*
 *	manage write line , meas it will check for line special chars and ...
 */
void write_line(line *);
/*
 *	this function will write a message into display, first it will check for
 *	syntax highlight , if there was any related prop with given text , first it
 *	will apply them then write text
 */
void print_line(char *);

/*
 *	when linenumber is enabled for buffer , before writing line , we will call this
 *	to write linenumber
 */
int write_linenumber(int,int);

/*
 *	used for check if buffers line count increased or not , for example when line-
 *	number increase from 99 to 100 , we have to increase buffer padding
 */
int update_linenumber_padding();

/*
 *	write status bar into screen , the place that we will put information about 
 *	current state of editor and cursor and file
 */
void write_statusbar(buffer *);

/*
 *	write message bar , and show prompt (if exist) and messages
 */
void write_messagebar();

/*
 *	show a message in messagebar
 */
void showmsg(bool,char *,...);

void clear_info_box();
void toggle_info_box();
void print_info_box();
void close_info_box();
void append_text_to_info_box(char *);

void set_block_cursor();
void set_bar_cursor();

#endif
