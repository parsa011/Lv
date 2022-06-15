#ifndef _DISPLAY_H
# define _DISPLAY_H

#define TABBAR_BG (create_bg_color(color_new(44, 44, 44)))
#define TABBAR_FG (create_fg_color(color_new(123, 123, 123)))

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
 *	print tab bar if that's enabled
 *  +--------------------------------------------------------------+
 *	| LV [ main.c ] | [ display.c ]                                |
 *  +--------------------------------------------------------------+
 */
public void update_tabbar();

/*
 *	set given color for all of the current line , that set cursor to it's prev position
 */
public void paint_line(char *color);

public void write_tildes();

/*
 *	take terminal window size then update 'global_editor' info
 */
public void update_screen_size();

#endif
