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
|12    TTmove(term.t_mrow,0);		     		 	 |
|13    TTputs("this is a test message"); 	     	 |
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
	TTputs(buf);
}

void update()
{
	TTchide();
	TTmove(0,0);
	write_windows();
	write_buffer();
	write_statusbar();
	write_messagebar();
	check_cursor();
	TTmove(cursor_row,cursor_col);
	TTcshow();
	TTflush();
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
	if (cursor_row != windowsbar_start_offset || cursor_col != 1)
		TTmove(windowsbar_start_offset,1);
	TTeeol();
	TTputs(INVERT);
	char temp[256];
	int len = sprintf(temp,"%s ",EDITOR_TITLE);
	TTputs(temp);
	/* later we have to check if we have any special color for windows section */
	for (window *wp = firstwp;wp != NULL;wp = wnext(wp)) {
		if (wp->fbuffer == NULL) {
			TTputs(NO_NAME_BUFFER);
			len += strlen(NO_NAME_BUFFER);
		}
		else {
			TTputs(wp->fbuffer->bname);	
			len += strlen(wp->fbuffer->bname);
		}	
		/* if this is not last window , wirte | separator */
		if (wnext(wp) != NULL) {
			TTputs(WINDOWS_SEPARATOR);
			len += sizeof(WINDOWS_SEPARATOR);
		}
	}
	while (len <= term.t_mcol)  {
		TTputs(" ");
		len++;
	}
	TTputs(DEFAULT);
	TTputs("\r\n");
}

/*
 * 	cursor position should be 1,0 to write buffer into screen
 */
void write_buffer()
{
	if (cursor_row != buffers_start_offset || cursor_col != 1)
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
	char *temp = strdup(ln->chars);
	while (*temp) {
		if (*temp == '\t') {
			for (int i = 0;i < tab_size;i++) {
				TTputc(' ');
			}
		} else 
			TTputc(*temp);
		*temp++;
	}	
	TTputs("\r");
}

/*
 *	write left status and right into status bar
 *	we use llen and rlen to detect size of wroted chars,
 *	first we will write lstatus , then increase llen ,
 *	until the sum of its size with the length on the right
 *	be equal to the size of terminal col, then we will write 
 *	other one but if sum of them is less than terminal col size , 
 *	we will put space to fill status bar
 */
void write_statusbar()
{
	if (cursor_row != statusbar_start_offset || cursor_col != 1)
		TTmove(statusbar_start_offset,1);
	TTeeol();
	TTputs(INVERT);
	char lstatus[256];
	char rstatus[128];
	int llen = sprintf(lstatus,"file : %s , %d line ",curbp->bname,curbp->lcount);
	int rlen = sprintf(rstatus," %d | %d - %d",curbp->clindex + 1,cursor_col,current_line->len);
	TTputs(lstatus);
	while (llen < term.t_mcol) {
		if (llen + rlen == term.t_mcol) {
			TTputs(rstatus);
			break;
		}
		TTputc('-');
		llen++;
	}
	TTputs("\r");
	TTputs(DEFAULT);
}

void write_messagebar()
{
	if (cursor_row != messagebar_start_offset || cursor_col != 1)
		TTmove(messagebar_start_offset,1);
	TTeeol();
	TTputs("this is a test message");
}
