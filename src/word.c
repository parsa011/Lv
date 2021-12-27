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
			if (prev_char(true, 1) != true)
				return false;
		}
		// escape of spaces , to go to the start of next word
		while (in_word() == false) {
			if (prev_char(true, 1) != true)
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
		for (int i = 0;i < curbp->nrow;i++)
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
		for (int i = 0;i < curbp->nrow;i++)
			if (!move_prevline(true,1))
				return false;
	}
	return true;
}

/*
 *	find twin char of current char under cursor
 *	if it was a twin char we will look for it's sibling
 *	otherwise just showing error to user that this is not twin char
 *	for check for if current char is twin , we use of twin_char array
 *	that it twin elements placecs side by side in rows , like this :
 *
 *	even	odd
 *	 {		 }
 *	 (		 )
 *
 * 	for finding sibling , we need to know what is current char , and
 * 	in what direction we have to move , if current char is even so we
 * 	have to move a char to rigth , otherwise to left.
 * 	also when we are moving , we will check new chars , if it was like 
 * 	first char , we will increase count variable , otherwise if it was
 * 	twin one , we will decrease. 
 * 	NOTE : sometimes siblings are same , like quote chars , we have to 
 * 	check for them when we want to increase count variable
 * 	at the end check count variable , if was 0 , so we founded twin char , 
 * 	and just break the loop. 
 * 	sometimes we cant find other one, means count is bigger than 0
 * 	so we will break the loop and will show a message to user that 
 * 	says : we cant find and ...
 *	this happens if we cant move any more(move next and prev will return false)
 *	when 'done' is false we have to back to last position(where the cursor was)
 */
int find_sibling(int f,int n)
{
	if (current_line == NULL)
		return empty_buffer();
	int cur_char_index = get_twin_char_index(lgetc(current_line,curbp->coffset));	
	if (cur_char_index == -1) {
		showmsg(true,"can not find sibling of this char");
		return false;
	}
	bool odd = cur_char_index % 2 != 0;
	char twin = twin_chars[odd ? cur_char_index - 1: cur_char_index + 1];
	bool same_char = twin == twin_chars[cur_char_index];
	int count = 1;
	bool done = true;
	int clindex = curbp->clindex;
	while (true) {
		if (odd) {
			if (prev_char(true,1) != true)
				done = false;
		}
		else {
			if (next_char(true,1) != true)
				done = false;
		}

		if (!same_char && lgetc(current_line,curbp->coffset) == twin_chars[cur_char_index])
			count++;
		else if (lgetc(current_line,curbp->coffset) == twin)
			count--;
		if (count == 0)
			break;
		if (done == false) {
			// TODO : goto line here for back to saved line
			showmsg(true,"can not find sibling , check out your code again :)");
			break;
		}
	}
	return done;
}

/*
 *	goto line number with n index
 */
int goto_line(int f,int n)
{
	if (n > curbp->lcount) {
		showmsg(false,"Oops ,What line ?");
		return OUTOFBUFFER;
	}
	cursor_row = buffers_start_offset;
	curbp->clindex = --n;
	line *ln = get_line_by_index(n);
	curbp->hline = curbp->cline = ln;
	curbp->flags |= FREDRW;
	return true;
}

int goto_end_of_buffer(int f,int n)
{
	goto_line(true,curbp->lcount);
	return true;
}

int goto_start_of_buffer(int f,int n)
{
	goto_line(true,1);
	return true;
}
