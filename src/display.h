#ifndef _DISPLAY_H
# define _DISPLAY_H

/*
 *	rewrite current window and also update tabbar and command-line section
 */
public void update_screen();

public void update_tabbar();

public void paint_line(char *color);

public void write_tildes();

void update_screen_size();

#endif
