#ifndef _DISPLAY_H
# define _DISPLAY_H

/* 
 *	time to second that we want to show messages to user
 */
#define USER_MESSAGE_TIME 2

/* #define TABBAR_BG (color_new(25, 55, 255)) */
/* #define TABBAR_FG (color_new(1, 1, 1)) */
#define TABBAR_FG (color_new(1, 1, 1))
#define TABBAR_BG (color_new(255, 255, 255))

#define TEXT_ROW_SIZE global_editor.term_row - global_editor.show_tabs - 2

/*
 *	rewrite current window and also update tabbar and command-line section
 */
public void update_screen();

/*
 *	Update text section , or it's better to say write buffer into screen
 *	TODO : handle splited views , but we dont have that yet, just told that
 */
public void update_text();

/*
 *	Write given line into output by converting tabs to space and syntax highlighting
 */
public void write_line(line *ln);

/*
 *	print tab bar if that's enabled
 *  +--------------------------------------------------------------+
 *	| LV [ main.c ] | [ display.c ]                                |
 *  +--------------------------------------------------------------+
 */
public void update_tabbar();

/*
 *	update command bar and show information and read commands 
 */
public void update_status_bar();

/*
 *	for now this functino will not write anything to command bar, it will check if
 *	message need to be erased or no bu checking its time 
 */
public void update_command_bar();

/*
 *	set given color for all of the current line , that set cursor to it's prev position
 */
public void paint_line(char *color);

/*
 *	take terminal window size then update 'global_editor' info
 */
public void update_screen_size();

#endif
