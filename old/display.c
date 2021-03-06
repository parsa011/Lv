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

static struct rectangle *info_box;
static bool show_info_box = false;

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
	get_screen_size(&term.t_mrow, &term.t_mcol);
	return true;
}

int init_display()
{
	info_box = init_rec(5, 5, "info box");
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
void set_terminal_title(char *title)
{
	char buf[250];
	int len = sprintf(buf,"\033]0;%s\007",title);
	TTputs(buf);
}

void update()
{
	TTchide();
	TTmove(1,1);
#if HAVE_WINDOWS_BAR
	write_windows();
#endif
	// check if buffer need to redraw ot not
	if (curbp->flags & FREDRW) {
		update_linenumber_padding();
		write_buffer();
		// cause we draw buffer again , so dont need it anymore , until another change
		curbp->flags &= ~FREDRW;
	}
	if (show_info_box)
		draw_rec(info_box);
	write_statusbar(curbp);
	write_messagebar();
	check_cursor();
	if (bmtest(curbp,MDCMMD))
		TTmove(messagebar_start_offset,msgbar_cursor_col);
	else {
		TTmove(cursor_row,cursor_col + curbp->mleft);
	}	
	TTcshow();
	TTflush();
}

/*
 *	write editor title on top and window names
 *	if window have buffer , we will use active buffer bname
 *	else will use NO_NAME_BUFFER const
 */
void write_windows()
{
	TTmove(windowsbar_start_offset,1);
	TTeeol();
	TTputs(INVERT);
	char temp[256];
	int len = sprintf(temp,"%s ",EDITOR_TITLE);
	TTputs(temp);
	/* later we have to check if we have any special color for windows section */
	for (window *wp = firstwp;wp != NULL;wp = L_LINK_NEXT(wp)) {
		if (wp->fbuffer == NULL) {
			TTputs(NO_NAME_BUFFER);
			len += strlen(NO_NAME_BUFFER);
		}
		else {
			TTputs(wp->fbuffer->bname);
			len += strlen(wp->fbuffer->bname);
		}
		/* if this is not last window , write | separator */
		if (L_LINK_NEXT(wp) != NULL) {
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
 * 	TODO : write good comment here
 */
void write_buffer()
{
	buffer *bf = curwp->fbuffer;
	for (buffer *bf = curwp->fbuffer; bf != NULL; bf = L_LINK_NEXT(bf)) {
		TTmove(bf->mtop,1);
		write_buffer_lines(bf);
		write_statusbar(bf);
	}
}

void write_buffer_lines(buffer *bf)
{
	int count = 0;
	int linenu = bf->loffset + 1;
	int linenu_offst = number_len(bf->lcount);
	bool linem = bf->linenm;
	for (line *ln = get_header_line(); count < bf->nrow; count++) {
		TTeeol();
		if (ln != NULL) {
			if (linem)
				write_linenumber(linenu++, linenu_offst);
			write_line(ln);
			ln = L_LINK_NEXT(ln);
		} else {
			TTeeol();
			TTputs(LINE_MASK);
			TTputs("\r\n");
		}
	}
}

/*
 *	write line into screen, convert non-printable chars into 
 *	something printable :) (not implemented yet)
 */
void write_line(line *ln)
{
	//TTeeol();
	char *temp = ln->chars;
	char *text_bag = lv_malloc(256);
	int i = 0;
	bool append = false;
	/* this is for control count of chars that we want to write into the screen , to dont let line be more than terminal
	   col size 
	 */
	int wroted_chars = 0;
	while (*temp && wroted_chars < term.t_mcol) {
		wroted_chars++;
		/* we will check if current char is space or end of line 
		 * if it was one of them , so we have to write text bag into screen
		 * also we have to check if we will reach end of file or no, by
		 * checking next char with temp + 1
		 */
		if (*temp == ' ' || *temp == '\n' || *temp == '\t') {
			append = true;
		} else if (iscntrl(*temp)) {
			*(text_bag + i++) = (*temp <= 26 ? '@' + *temp : '?');
		} else {
			if (isalpha(*temp) || *temp == '#')
				*(text_bag + i++) = (*temp);
			else
				append = true;
		}
		*(text_bag + i) = '\0';
		/* check if we need to append , or we are at end of line */
		if (!*(temp + 1) || append) {
			print_line(text_bag);
			TTputs(DEFAULT);
			if (append) {
    			if (*temp == '\t') {
                    for (int j = 0;j < tab_size;j++) {
            			/* here we can show tabs if needed :)) */
        				//if (j == tab_size / 2)
        				//	TTputc('.');
        				//else 
        				TTputc(' ');
                    }
    			} else
        			TTputc(*temp);
			}
			append = false;
			i = 0;
		}
		*temp++;
	}
	free(text_bag);
	TTputs("\n\r");
}

/*
 *	write line into screen , also it will search for color for given text
 * 	if syntax is enabled
 */
void print_line(char *text)
{
	if (curbp->highligth) {
		char **props = get_syntax_for_keyword(text);
		if (props) {
			while (*props) {
				TTputs(prop_to_str(*props++));
			}
		}
	}
	TTputs(text);
}	

/*
 *	write line number into  screen
 *	number is current line number and offset is value of
 *	line number section len(width)
 */
int write_linenumber(int number,int offset)
{
	char temp[number];
	int len = sprintf(temp, "%d", number);
	TTputs(temp);
	while (len++ <= offset - 1) {
		TTputc(' ');
	}
	TTputs("\u2502");
}

/*
 *	update padding of buffer , caculate len of buffer lines count
 *	and plus one one a space(separation line number of buffer lines)
 */
int update_linenumber_padding()
{
	if (curbp->lcount == 0) {
		curbp->mleft = 0;
		return 0;
	}
	int len = number_len(curbp->lcount);
	len++; // just a little space for separation
	curbp->mleft = curbp->linenm ? len : 0;
	return len;
}

/*
 *	write left status and right into status bar
 *	we use llen and rlen to detect size of wroted chars,
 *	first we will write lest status , then increase llen ,
 *	until the sum of its size with the length on the right
 *	be equal to the size of terminal col, then we will write
 *	other one but if sum of them is less than terminal col size ,
 *	we will put space to fill status bar
 */
void write_statusbar(buffer *bf)
{
	TTmove(bf->mtop + bf->nrow - 1,1);
	TTeeol();
	TTputs(INVERT);
	char lstatus[256];
	char rstatus[128];
	int llen = sprintf(lstatus, "file : %s , %d line ", bf->bname[0] != 0 ? bf->bname : NO_NAME_BUFFER, bf->lcount);
	int rlen = sprintf(rstatus, " %s %c%c %d | %d - %d",
			bmtest(bf, MDLOCK) ? "lock" : "insert", STATUSBAR_FILLER, STATUSBAR_FILLER,
			bf->clindex + 1, bf->coffset + 1, current_line != NULL ? current_line->len : 0);
	TTputs(lstatus);
	while (llen < term.t_mcol) {
		if (llen + rlen == term.t_mcol) {
			TTputs(rstatus);
			break;
		}
		TTputc(STATUSBAR_FILLER);
		llen++;
	}
	TTputs(DEFAULT);
	TTputs("\n\r");
}

void write_messagebar()
{
	TTmove(messagebar_start_offset, 1);
	TTeeol();
	if (bmtest(curbp,MDCMMD)) {
		TTputs(msgbar_prompt_mask);
		TTputs(msgbar_prompt);
		TTputs("  ");
	}
	if (msgbag.timer) {
		if (time(NULL) - msgbag.msg_time < msgbar_msg_time) {
			TTputs(msgbag.message);		
		} 
	} else {
		TTputs(msgbag.message);
		msgbag.message[0] = '\0';
	}
}

/*
 *	write msg to msgbag messages and set timer for
 *	it if needed
 */
void showmsg(bool timer, char *msg,...)
{
	TTmove(messagebar_start_offset, 1);
	msgbag.timer = timer;
	if (timer) {	
		msgbag.msg_time = time(NULL);
	}
	va_list ap;
	va_start(ap, msg);
	vsnprintf(msgbag.message, MESSAGE_MAX_LENGTH, msg, ap);
	va_end(ap);
}

/*
 *	clear info box texts and reset it size
 */
void clear_info_box()
{
    assert(info_box);
	info_box->info->width = info_box->info->height = 5;
	info_box->buf_size = 10;
	info_box->text_count = 0;
	memset(info_box->texts, 0, 10);
}

/*
 *	set show_info_box true for print info_box in each 'update' function call.
 *	and also read-time show
 */ 
void print_info_box()
{
	show_info_box = true;
	redisplay_buffer();
}

/*
 * hide info box and rewrite buffer into screen for erase infobox
 */
void close_info_box()
{
	show_info_box = false;
	clear_info_box();
	redisplay_buffer();
}

/*
 *	append given char into info_box.
 *	we juse append text to text , will not display it
 */
void append_text_to_info_box(char *txt)
{
	append_string_to_rec(info_box, txt);
}

void set_block_cursor()
{
	TTputs("\x1b[\x32 q");
}

void set_bar_cursor()
{
	TTputs("\x1b[\x36 q");
}
