/*
 *	Manager cursor and position of screen
 *	Copyright
 *		(C) 2021 Parsa Mahmoudy sahebi
 *
 *	This file is part of Lv
 */
#include "types.h"

void move_cursor()
{
	check_cursor();
	TTmove(cursor_row,cursor_col);
}

void check_cursor()
{
	if (cursor_row > curbp->mtop + curbp->nrow) {
		goto_line(true,curbp->clindex);
	}

	if (cursor_col <= 0) {
		cursor_col = 1;
		curbp->coffset = 0;
	}

	if (current_line == NULL)
		return;
	
	/* if cursor is out the line , set it to line length + 1 */
	if (curbp->coffset > current_line->len || cursor_col > line_length(current_line) + 1) {
		cursor_col = line_length(current_line) + 1;
		curbp->coffset = current_line->len;
	}
}

void update_position()
{
	curbp->coffset = convert_cursorcol_to_coffset(current_line->chars,cursor_col);
	cursor_col = convert_coffset_to_cursorcol(current_line->chars,curbp->coffset);
}

/*
 *	scroll down page , to specified times
 */
int scroll(int dir, int times)
{
	for (;0 < times;times--) {
		line *header_line = get_header_line();
		if (dir == MOVE_DOWN) {
			if (L_LINK_NEXT(header_line) != NULL) {
				set_header_line(++curbp->loffset);
			}
		} else if (dir == MOVE_UP) {
			if (L_LINK_PREV(header_line) != NULL) {	
				set_header_line(--curbp->loffset);
			}
		}
	}
	redisplay_buffer();
	return 1; 
}

bool can_scroll(int dir)
{
	if (dir == MOVE_DOWN && cursor_row == curbp->mtop + curbp->nrow - 2)
		return true;
	else if (dir == MOVE_UP && cursor_row == curbp->mtop)
		return true;
	return false;
}

/*
 * 	move cursor to next line
 * 	and also check if we are not at start of buffer (very first line)
 * 	and if cursro_row + 1 will goes into next section , instead of
 * 	moving next line , will scroll down
 */
int move_nextline(int f, int n)
{
	while (n--) {
		if (curbp->lcount == 0)
			return empty_buffer();
		if (L_LINK_NEXT(current_line) == NULL)
			return endof_buffer();
		if (can_scroll(MOVE_DOWN)) {
			scroll(MOVE_DOWN, 1);
		} else
			cursor_row++;
		curbp->clindex++;
		current_line = L_LINK_NEXT(current_line);
		update_position();
		move_cursor();
	}
	return true;
}

/*
 *	move cursor to next line , but before that will check if we are not 
 *	at the end of buffer
 *	and also check if cursor_row - 1 will goes to prev section in editor
 *	if it goes , it will scroll top instead of moving cursor
 */
int move_prevline(int f, int n)
{
	while (n--) {
		if (current_line == NULL || curbp->lcount == 0)
			return empty_buffer();
		if (L_LINK_PREV(current_line) == NULL)
			return topof_buffer();
		if (can_scroll(MOVE_UP)) {
			scroll(MOVE_UP,1);
		} else 
			cursor_row--;
		curbp->clindex--;
		current_line = L_LINK_PREV(current_line);
		update_position();
		move_cursor();
	}
	return true;
}

int next_char(int f, int n)
{
	while (n--) {
		if (current_line == NULL || curbp->lcount == 0)
			return empty_buffer();
		if (curbp->coffset < current_line->len) {
			if (jump_tab(MOVE_RIGHT) == false)
				cursor_col++;
			curbp->coffset++;
			move_cursor();
		} else {
			/*
			 *	ok , at this point , we will check for next line , of its NULL , so we are at the end of buffer , just return false
			 *	otherwise go to next line and set cursor col to 1
			 */
			if (L_LINK_NEXT(current_line) == NULL)
				return endof_buffer();
			cursor_col = 1;
			curbp->coffset = 0;
			move_nextline(true,1);
		}
	}
	return true;
}

int prev_char(int f, int n)
{
	while (n--) {
		if (current_line == NULL || curbp->lcount == 0)
			return empty_buffer();
		if (curbp->coffset > 0) {
			curbp->coffset--;
			if (jump_tab(MOVE_LEFT) == false)
				cursor_col--;
			move_cursor();
		} else {
			/*
			 * 	well , if prev line is NULL, so we are at the top of buffer , we just have to return
			 * 	otherwise we will call move_cursor with move up argument
			 * 	then set cursor col to next line (it's current line now) length
			 */
			if (L_LINK_PREV(current_line) == NULL)
				return topof_buffer();
			move_prevline(true,1);
			cursor_col = line_length(current_line) + 1; 
			curbp->coffset = current_line->len;
		}
	}
	return true;
}

bool jump_tab(int dir)
{
	if (lgetc(current_line, curbp->coffset) == '\t') {
		if (dir == MOVE_RIGHT) {
			cursor_col += tab_size;
		} else if (dir == MOVE_LEFT) {
			cursor_col -= tab_size;
		}
		return true;
	}
	return false;
}
