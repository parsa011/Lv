/*
 * 	This file deal with tasks that are related to word
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

/*
 * check if the cursor is in a word
 */
int in_word()
{
	if (current_line == NULL)
		return false;
	int c;
	if (curbp->coffset > current_line->len)
		return false;
	c = lgetc(current_line,curbp->coffset);
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return true;
	return false;
}

int forwword(int f, int n)
{
	if (n < 0)
		return backword(f,-n);
	while (n--) {
		// escape of chars
		while (in_word() == true) {
			if (next_char(false, 1) != true)
				return false;
		}
		// escape of spaces , to go to the start of next word
		while (in_word() == false) {
			if (next_char(false, 1) != true)
				return false;
		}
	}
	return true;
}

int backword(int f, int n)
{
	if (n < 0)
		return forwword(f,-n);
	while (n--) {
		// escape of chars
		while (in_word() == true) {
			if (prev_char(false, 1) != true)
				return false;
		}
		// escape of spaces , to go to the start of next word
		while (in_word() == false) {
			if (prev_char(false, 1) != true)
				return false;
		}
	}
	return true;
}

/*
 *	move cursor to end of line
 */
int gotoeol(int f, int n) 
{
	if (current_line == NULL || curbp->lcount == 0)
		return empty_buffer();
	curbp->coffset = current_line->len;
	cursor_col = line_length(current_line) + 1;
	return true;
}

/*
 *	move cursor to start of line
 */
int gotosol(int f, int n) 
{
	if (current_line == NULL || curbp->lcount == 0)
		return empty_buffer();
	curbp->coffset = 0;
	cursor_col = 1;
	return true;
}

/*
 *	move to next page , scroll down until reach it :)
 */
int move_nextpage(int f, int n)
{
	if (n < 0)
		return move_prevpage(f,-n);
	while (n--) {
		int temp = statusbar_start_offset - windowsbar_start_offset; // how many row we have for buffers	
		for (int i = 0;i < temp;i++)
			if (!move_nextline(true,1))
				return false;
	}
	return true;
}

int move_prevpage(int f,int n)
{
	if (n < 0)
		return move_nextpage(f,-n);
	while (n--) {
		int temp = statusbar_start_offset - windowsbar_start_offset; // how many row we have for buffers	
		for (int i = 0;i < temp;i++)
			if (!move_prevline(true,1))
				return false;
	}
	return true;
}
