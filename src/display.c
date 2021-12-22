/*
 *	Handle redisplay
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

#include <sys/ioctl.h>
#include <stdarg.h>
#include <time.h>

/*
|----------------------------------------------------|
|PMS 0.0.1  [NO NAME]                                |
|----------------------------------------------------|
|10 void write_messagebar()                          |
|11 {                                                |
|12    TTmove(term.t_mrow,0);                        |
|13    TTputs("this is a test message");             |
|14 }                                                |
|----------------------------------------------------|
|display.c | 100 line | 10.1                 c type  |
|----------------------------------------------------|
|381 byte wroted                                     |
|----------------------------------------------------|
*/

/*
 *	initialize terminal and trun it into raw mode
 *	and other basic configurations for terminal :)
 */
int init_term()
{
	TTopen();
	if (system("clear") != 0)
		TTbeep();
	TTmove(0,0);
	/* update term global variable row and col */
	get_screen_size(&term.t_mrow,&term.t_mcol);
}

/*
 *	get terminal size from system
 *	if returned value from ioctl was valid ( more than 0 )
 *	we will set row and col by winsize row and col
 *	but if return value from ioctl wan not valid , we will
 *	move the cursor to the lowest corner of the screen , the request the
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
 *	return false if we can't get the cursor position by any accident
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
	if (bmtest(curbp,MDCMMD))
		TTmove(messagebar_start_offset,msgbar_cursor_col);
	else
		TTmove(cursor_row,cursor_col);
	TTcshow();
	TTflush();
	// ===========================================
	// + set cursor to 0,0
	// + write windows on top (tabs)
	// + write buffer into screen
	// + write status bar
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
		/* if this is not last window , write | separator */
		if (wnext(wp) != NULL) {
			TTputs(WINDOWS_SEPARATOR);
			len += sizeof(WINDOWS_SEPARATOR);
		}
	}
	while (len < term.t_mcol)  {
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
	for (line *ln = curbp->hline;count < statusbar_start_offset - buffers_start_offset - 1;count++) {
		if (ln != NULL) {
			write_line(ln);
			ln = lnext(ln);
		} else {
			TTeeol();
			TTputs(LINE_MASK);
			TTputs("\r\n");
		}
	}
}

/*
 *	write line into screen, convert non-printable chars into something printable :) (not implemented yet)
 */
void write_line(line *ln)
{
	TTeeol();
	char *temp = ln->chars;
	while (*temp) {
		if (*temp == '\t') {
			for (int i = 0;i < tab_size;i++) {
				TTputc(' ');
			}
		} else
			TTputc(*temp);
		*temp++;
	}
	TTputs("\r\n");
}

/*
 *	write left status and right into status bar
 *	we use llen and rlen to detect size of wrote chars,
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
	int rlen = sprintf(rstatus," %s --- %d | %d - %d",
			bmtest(curbp,MDLOCK) ? "+ lock" : "! insert",curbp->clindex + 1,curbp->coffset + 1,current_line != NULL ? current_line->len : 0);
	TTputs(lstatus);
	while (llen < term.t_mcol) {
		if (llen + rlen == term.t_mcol) {
			TTputs(rstatus);
			break;
		}
		TTputc('-');
		llen++;
	}
	TTputs("\r\n");
	TTputs(DEFAULT);
}

void write_messagebar()
{
	if (cursor_row != messagebar_start_offset || cursor_col != 1)
		TTmove(messagebar_start_offset,1);
	TTeeol();
	if (bmtest(curbp,MDCMMD)) {
		TTputs(":");
		TTputs(msgbar_prompt);
	}
	if (msgbag.timer) {
		if (time(NULL) - msgbag.msg_time < msgbar_msg_time) {
			TTputs(msgbag.message);		
		} 
	} else 
		TTputs(msgbag.message);
}

void showmsg(bool timer, char *msg,...)
{
	if (cursor_row != messagebar_start_offset || cursor_col != 1)
		TTmove(messagebar_start_offset,1);
	if (timer) {
		msgbag.timer = true;
		msgbag.msg_time = time(NULL);
	}
	va_list ap;
	va_start(ap, msg);
	vsnprintf(msgbag.message, MESSAGE_MAX_LENGTH, msg, ap);
	va_end(ap);

}
