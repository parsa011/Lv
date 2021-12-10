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

	if (lv_write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return false;

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
 * move cursor to specified location 
 */
bool move_cursor(int row, int col)
{
	curbp->crow = row;
	curbp->ccol = col;
	TTmove(row,col);
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

void update()
{
	move_cursor(0,0);
	write_windows();
	write_buffer();
	write_statusbar();
	write_messagebar();

	// ===========================================
	// + set cursor to 0,0
	// + wirte windows on top (tabs)
	// + write buffer into screen
	// + wrtite status bar
	// create a place for message bar
	// ===========================================
}

/*
 *	write editor title on top and window names
 *	if window have buffer , we will use active buffer bname
 *	else will use NO_NAME_BUFFER const
 */
void write_windows()
{
	if (curbp->crow != 0 || curbp->ccol != 0)
		move_cursor(0,0);
	TTputc(INVERT);
	int temp = 0;
	while (temp < term.t_mcol) {
		TTputc(" ");
		temp++;
	}
	move_cursor(1,0);
	TTputc(EDITOR_TITLE);
	TTputc("\t");
	/* later we have to check if we have any special color for windows section */
	for (window *wp = firstwp;wp != NULL;wp = wnext(wp)) {
		if (wp->fbuffer == NULL)
			TTputc(NO_NAME_BUFFER);
		else
			TTputc(wp->fbuffer->bname);	
		/* if this is not last window , wirte | separator */
		if (wnext(wp) != NULL)
			TTputc(" | ");
	}
	TTputc(DEFAULT);
	TTputc("\r\n");
}

/*
 * 	cursor position should be 1,0 to write buffer into screen
 */
void write_buffer()
{
	if (curbp->crow != 1 || curbp->ccol != 0)
		move_cursor(2,0);
	int count = 0;
	for (line *ln = curbp->fline;ln != NULL && count < term.t_mrow - 2;ln = lnext(ln),count++) {
		TTputc(ln->chars);
		TTputc("\r");
	}
}

void write_statusbar()
{
	if (curbp->crow != term.t_mrow - 1  || curbp->ccol != 0)
		move_cursor(term.t_mrow - 1,0);
	TTputc(INVERT);
	int temp = 0;
	while (temp < term.t_mcol) {
		TTputc(" ");
		temp++;
	}	
	TTputc(DEFAULT);

}

void write_messagebar()
{
	if (curbp->crow != term.t_mrow || curbp->ccol != 0)
		move_cursor(term.t_mrow,0);
	TTputc("this is a test message");
}
