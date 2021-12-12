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
|----------------------------------------------------|
|PMS 0.0.1  [NO NAME]				     			 |
|----------------------------------------------------|
|10 void write_messagebar()			     			 |
|11 {						     					 |
|12    TTmove(term.t_mrow,0);		     		 |
|13    TTputc("this is a test message"); 	     	 |
|14 }					             				 |	
|----------------------------------------------------|
|display.c | 100 line | 10.1		     	 c type  |
|----------------------------------------------------|
|381 byte wroted				     				 |
|----------------------------------------------------|
*/

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

	if (lv_ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
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
	TTchide();
	TTmove(0,0);
	write_windows();
	write_buffer();
	write_statusbar();
	write_messagebar();
	TTmove(curbp->crow,curbp->ccol);
	TTcshow();

	// ===========================================
	// + set cursor to 0,0
	// + wirte windows on top (tabs)
	// + write buffer into screen
	// + wrtite status bar
	// + create a place for message bar
	// ===========================================
}

/*
 *	write editor title on top and window names
 *	if window have buffer , we will use active buffer bname
 *	else will use NO_NAME_BUFFER const
 */
void write_windows()
{
	if (curbp->crow != windowsbar_start_offset || curbp->ccol != 1)
		TTmove(windowsbar_start_offset,1);
	TTeeol();
	TTputc(INVERT);
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
	if (curbp->crow != buffers_start_offset || curbp->ccol != 1)
		TTmove(buffers_start_offset,1);
	int count = 0;
	for (line *ln = curbp->hline;ln != NULL && count < term.t_mrow - 2;ln = lnext(ln),count++) {
		write_line(ln);
	}
}

/*
 *	write line into screen, convert non-printable chars into something printable :) (not implemented yet)
 */
void write_line(line *ln)
{
	TTeeol();
	TTputc(ln->chars);
	TTputc("\r");
}

void write_statusbar()
{
	if (curbp->crow != statusbar_start_offset || curbp->ccol != 1)
		TTmove(statusbar_start_offset,1);
	TTeeol();
	TTputc(INVERT);
	char lstatus[256];
	char rstatus[128];
	int llen = sprintf(lstatus,"file : %s , %d line %d col",curbp->bname,curbp->lcount,term.t_mcol);	
	int rlen = sprintf(rstatus,"%d | %d",curbp->clindex + 1,cursor_col);
	TTputc(lstatus);
	while (llen < term.t_mcol) {
		if (llen + rlen == term.t_mcol) {
			TTputc(rstatus);
			break;
		} 
		TTputc(" ");
		llen++;
	}
	TTputc("\r");
	TTputc(DEFAULT);
}

void write_messagebar()
{
	if (curbp->crow != messagebar_start_offset || curbp->ccol != 1)
		TTmove(messagebar_start_offset,1);
	TTeeol();
	TTputc("this is a test message");
}
