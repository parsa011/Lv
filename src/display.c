/*
 *	Handle redisplay
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

#include <sys/ioctl.h>

/*
 *	get termianl size from system
 *	if returned value from ioctl was valid ( more than 0 )
 *	we will set row and col by winsize row and col
 *	but if return value from ioctl wan not valid , we will 
 *	move the cursor to lowest corner of the screen , the request the 
 *	cursor position
 */
void get_screen_size(int *wrow,int *wcol)
{
	struct winsize ws;

	if (lv_ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		TTmove(999,999);
		get_cursor_position(wrow,wcol);
	} else {
		*wcol = ws.ws_col;
		*wrow = ws.ws_row;
	}
}

/*
 *	return cursor position on screen
 *	return false if we cant get the cursor position by any accident
 */
bool get_cursor_position(int *rows, int *cols) 
{
	char buf[32];
	unsigned int i = 0;

	if (lv_write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return false;

	while (i < sizeof(buf) - 1) {
		if (lv_read(STDIN_FILENO, &buf[i], 1) != 1) 
			break;
		if (buf[i] == 'R') 
			break;
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') 
		return false;
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2)
		return false; 

	return true;
}

/*
 *	set terminal title
 */
void set_window_title(char *title)
{
	char buf[250];
	int len = sprintf(buf,"\033]0;%s\007",title);
	ttputc(buf);
}
